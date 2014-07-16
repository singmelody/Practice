using System;

namespace Client.UI
{
    public class UISystem : GameSystem
    {
        public UISystem(GameApp app)
            :base(app)
        {

        }
        public override void Init()
        {
            m_joyStickPanel = new JoyStickPanel("JoyStickPanel", this);



            m_joyStickPanel.Init();
        }

        private JoyStickPanel m_joyStickPanel = null;
    }
}