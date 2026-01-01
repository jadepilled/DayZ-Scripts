class ColdPack : ItemBase
{
        override bool CanHaveTemperature()
        {
                return true;
        }

        override void OnWorkStart()
        {
                if (GetGame().IsServer())
                {
                        SetTemperatureDirect(-1);
                }
        }

        override void OnWork(float consumed_energy)
        {
                if (GetGame().IsServer())
                {
                        SetTemperatureDirect(-1);
                }
        }

        override void OnWorkStop()
        {
                if (GetGame().IsServer())
                {
                        SetHealth(0);
                }
        }

        override void SetActions()
        {
                super.SetActions();

                AddAction(ActionTurnOnColdPack);
        }

        override float GetQuantityNormalizedScripted()
        {
                return 1.0;
        }

        override bool IsSelfAdjustingTemperature()
        {
                return GetCompEM().IsWorking();
        }
}
