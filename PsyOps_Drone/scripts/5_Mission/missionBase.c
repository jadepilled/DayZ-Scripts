modded class MissionBase
{


    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        UIScriptedMenu DroneBuyMenu = NULL;
        DroneBuyMenu = super.CreateScriptedMenu(id);
        if (!DroneBuyMenu)
        {
            switch (id)
            {
                case DRONEBUYMENUUI:
                    DroneBuyMenu = new DroneBuyMenuUI();
                    break;
            }
            if (DroneBuyMenu)
            {
                DroneBuyMenu.SetID(id);
            }
        }
        return DroneBuyMenu;
    }
}
