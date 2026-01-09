class CfgMods
{
    class ItemTags
    {
        dir="ItemTags";
        picture="";
        action="";
        hideName=1;
        hidePicture=1;
        name="Item Tags";
        credits="";
        author="";
        authorID="";
        version="1.0";
        extra=0;
        type="mod";
        dependencies[]={"Game","World","Mission"};
        class defs
        {
            class gameScriptModule
            {
                value="";
                files[]={"ItemTags/scripts/3_game"};
            };
            class missionScriptModule
            {
                value="";
                files[]={"ItemTags/scripts/5_mission"};
            };
        };
    };
};
class CfgPatches
{
    class ItemTags
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[]={"DZ_Data"};
    };
};
