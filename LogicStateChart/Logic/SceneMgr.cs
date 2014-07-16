using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using RPGData;

namespace Logic
{
    public class SceneMgr : Singleton<SceneMgr>
    {

        public void Init()
        {
            GamePause = true;

            player = new Player();
            player.Init();
        }

        public bool GamePause
        {
            get
            {
                return m_bGamePause;
            }
            set
            {
                m_bGamePause = value;
            }
        }

        public Player player;

        private bool m_bGamePause;      //游戏是否暂停

    }
}