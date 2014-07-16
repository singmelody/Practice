using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using RPGData;
using Logic;
using MagicPads;
using Client;
namespace ScriptRuntime
{
    public class ScriptRoot : RuntimeScriptRoot
    {
        public override void Load()
        {

        }

        //private GameApp m_gameApp = null;
        private static ScriptRoot root = null;
        public static ScriptRoot Root
        {
            get
            {
                return root;
            }
        }

        public MagicPad MagicPad
        {
            get
            {
                return m_magicPad;
            }
        }

        private ScriptRoot()
        {
            root = this;
        }
        private void Init()
        {

            int screen_width = GraphicSystem.GetWidth();
            int screen_height = GraphicSystem.GetHeight();

            m_magicPad.Init(0, 0, screen_width, screen_height);

            //配置静态数据
            NPCAnimationConfig.Instance.Init();
            EnemyAnimationConfig.Instance.Init();
            //AllNPCChatConfig.Instance.Init();

			// 初始化消息管理器
			MessageDispatcher.Instance.Init();

            //TODO AutoMove
            //RecastFacadeMgr.Instance.Init();

            //逻辑功能类
            BBColliderMgr.Instance.Init();
            MoveMgr.Instance.Init();
            PlayerChatMgr.Instance.Init();
            EffectMgr.Instance.Init();
			SoundMgr.Instance.Init();

			//场景管理
			SceneMgr.Instance.Init();

            //逻辑动态数据
			EnemyMgr.Instance.Init();
            NPCMgr.Instance.Init();
            CameraMgr.Instance.Init();
            InputMgr.Instance.Init();

        }

        private void Clear()
        {
            //TODO AutoMove
            //RecastFacadeMgr.Instance.Clear();
            SceneMgr.Instance.player.Clear();
            NPCMgr.Instance.Clear();
            BBColliderMgr.Instance.Clear();
            UserDefGUI.UserDefGUIRoot.Instance.ClearUI();
        }

        private void Reset()
        {
            //TODO AutoMove
            //RecastFacadeMgr.Instance.Reset();
            SceneMgr.Instance.player.Reset();
            NPCMgr.Instance.Reset();
            CameraMgr.Instance.Reset();
            EffectMgr.Instance.Reset();
			SoundMgr.Instance.Reset();
            EnemyMgr.Instance.Reset();
            UserDefGUI.UserDefGUIRoot.Instance.Reset();
        }

        public override void Tick()
        {
            if (m_bIsFirstTick)
            {
                Init();
                m_bIsFirstTick = false;
            }

            float fElapseTime = Util.GetDeltaTime();

            InputMgr.Instance.Tick();

            if (SceneMgr.Instance.GamePause)
                return;

            CameraMgr.Instance.Tick();
            BBColliderMgr.Instance.Tick();
            SceneMgr.Instance.player.Update();
            EnemyMgr.Instance.Tick();
        }

        private bool m_bIsFirstTick = true;
        private MagicPad m_magicPad = new MagicPad();
    };
}