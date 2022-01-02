#include "00_FunctionRegister.h"
#include "SPInput.h"
#include "PlayerProcess.h"
#include "NormalCrystal.h"
#include "SleepCrystal.h"
#include "BulletProcess.h"

void RegisterAllFuncPtr(ObjectFactory* _factory)
{
    RegisterSPInput(_factory);
    RegisterPlayerProcess(_factory);
    RegisterNormalCrystal(_factory);
    RegisterSleepCrystal(_factory);
    RegisterBulletProcess(_factory);
}