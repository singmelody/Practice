using System;
using Client.UI;
namespace Client
{
    public class GameApp
    {
        public GameApp()
        {
        }

        public void Start()
        {

            m_inputSystem = new InputSystem(this);
            m_worldSystem = new WorldSystem(this);
            m_worldView = new WorldView();

            m_bridgeToLogic = new BridgeToLogic(m_inputSystem);
            m_UISystem = new UISystem(this);


            m_worldView.Init();
            m_inputSystem.Init();
            m_worldSystem.Init();
            m_UISystem.Init();
        }

        public WorldView WorldView
        {
            get
            {
                return m_worldView;
            }
        }


        private WorldView m_worldView = null;
        private UISystem m_UISystem = null;
        private InputSystem m_inputSystem = null;
        private WorldSystem m_worldSystem = null;
        private BridgeToLogic m_bridgeToLogic = null;
    }
}