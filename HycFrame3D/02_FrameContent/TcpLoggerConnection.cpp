#include "TcpLoggerConnection.h"

#include "PrintLog.h"

#include <FormatUtility.h>

namespace tcp = hyc::tcp;
namespace str = hyc::str;

std::list<std::string> TcpLoggerConnection::LogInfoList = {};

TcpLoggerConnection::TcpLoggerConnection()
    : TcpConnectionInterface(), SocketPtr({}), ConnectAddress({}),
      LoggerProcessInfo({}) {}

TcpLoggerConnection::~TcpLoggerConnection() {}

void TcpLoggerConnection::insertNewLogMessage(const std::string &LogMessage) {
  LogInfoList.push_back(LogMessage);
}

bool TcpLoggerConnection::createConnection() {
  STARTUPINFO SI = {};
  ZeroMemory(&SI, sizeof(SI));
  ZeroMemory(&LoggerProcessInfo, sizeof(LoggerProcessInfo));
  SI.cb = sizeof(SI);

  char CmdLine[] = {"04_HycLogger.exe"};
  BOOL ProcessCreateFlag = CreateProcessA(NULL, CmdLine, NULL, NULL, FALSE, 0,
                                          NULL, NULL, &SI, &LoggerProcessInfo);

  if (!ProcessCreateFlag) {
    auto Error = GetLastError();
    assert(ProcessCreateFlag && "fail to create HycLogger");
    (void)Error;
    return false;
  }

  SocketPtr = tcp::TcpSocket::create(tcp::ADDRFAM::INET);
  ConnectAddress = tcp::createIPv4FromString("127.0.0.1:32580");
  int ConnectError = SocketPtr.get()->connect(*ConnectAddress);
  bool Result = ConnectError ? false : true;

  return Result;
}

void TcpLoggerConnection::terminateConnection() {
  SocketPtr->sendAs<std::string>("/STOP_LOGGER");
  WaitForSingleObject(LoggerProcessInfo.hProcess, INFINITE);
  CloseHandle(LoggerProcessInfo.hProcess);
  CloseHandle(LoggerProcessInfo.hThread);
}

void TcpLoggerConnection::executeConnection() {
  static int FrameCount = 0;
  P_LOG1("game frame by new P_LOG : {}", FrameCount++);
  for (const auto &LogMessage : LogInfoList) {
    SocketPtr->sendAs<std::string>(LogMessage);
  }
  LogInfoList.clear();
}
