using System;
using ScriptGUI;
using ScriptRuntime;
using RPGData;
using Logic;
namespace UserDefGUI
{
    public class UserDefGUIRoot : GUIRoot
    {
        static UserDefGUIRoot m_instance;
        static public UserDefGUIRoot Instance
        {
            get
            {
                return m_instance;
            }
        }

        private UserDefGUIRoot()
        {
            if (null == m_instance)
            {
                m_instance = this;
            }
        }

        private ConstUI_City m_constUICityPanel = new ConstUI_City();
        private JoyStick m_JoyStickPanel = new JoyStick();
        private MainMenu m_mainMenu = new MainMenu();
        private ContinueMenu m_continueMenu = new ContinueMenu();
        private RestartMenu m_restartMenu = new RestartMenu();
        private BattleUI m_BattleUI = new BattleUI();
        private LoadingPanel m_loadingPanel = new LoadingPanel();
        private SkillPanel m_skillPanel = new SkillPanel();
        private CameraControlPanel m_cameraControlPanel = new CameraControlPanel();
        
        public override void Init()
        {
            base.Init();
            LoadLayout();
        }

        private void LoadLayout()
        {
            m_constUICityPanel.Init();
            m_JoyStickPanel.Init();
            m_mainMenu.Init();
            m_continueMenu.Init();
            m_restartMenu.Init();
            m_BattleUI.Init();
            m_loadingPanel.Init();
            m_skillPanel.Init();
            m_cameraControlPanel.Init();
            PlayerChatPanelMgr.Instance.Init();
        }

        public bool JoyStickIsCtrl()
        {
            return m_JoyStickPanel.IsCtrl;
        }

        public Vector2 JoystickDir
        {
            get
            {
                return m_JoyStickPanel.JoystickDir;
            }
        }

        public bool AttackButtonPressed
        {
            get
            {
                return m_skillPanel.AttackButtonPressed;
            }
        }

        public void SetContinueMenuVis(bool vis)
        {
            m_continueMenu.SetVisable(vis);
        }

        public void SetRestartMenuVis(bool vis)
        {
            m_restartMenu.SetVisable(vis);
        }

        public void SetConstUI_CityVis(bool vis)
        {
            m_constUICityPanel.SetVisable(vis);
        }

        public void SetJsPanelVis(bool vis)
        {
            m_JoyStickPanel.SetVisable(vis);
        }

        public void SetBattleUIVis(bool vis)
        {
            m_BattleUI.SetVisable(vis);
        }

        public void SetLoadingPanelVis(bool vis)
        {
            m_loadingPanel.SetVisable(vis);
        }

        public void ClearUI()
        {
            SetConstUI_CityVis(false);
            SetJsPanelVis(false);
            SetBattleUIVis(false);
        }

        public void ShowCityUI()
        {
            SetConstUI_CityVis(true);
            SetJsPanelVis(true);
        }

        public void ShowBattleUI()
        {
            SetBattleUIVis(true);
        }

        public void Reset()
        {
            if (true)
            {
                ShowBattleUI();
            }
            //else 
            //{
            //    ShowCityUI();
            //}
        }

    }
}