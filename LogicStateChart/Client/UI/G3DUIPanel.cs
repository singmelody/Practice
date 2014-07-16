using System;
using ScriptGUI;
namespace Client.UI
{
    public abstract class G3dUIPanel : UIPanel
    {
        public G3dUIPanel(string name, UISystem parent)
            : base(name, parent)
        {
            m_keyName = name + "_g3d";
        }

        public FString KeyName
        {
            get
            {
                return KeyName;
            }
        }

        private FString m_keyName;
    }


}