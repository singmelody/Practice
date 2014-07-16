using System;

namespace Client
{

    public abstract class GameSystem
    {
        public GameSystem(GameApp parent)
        {
            m_root = parent;
        }
        public virtual void Init()
        {
        }

        public virtual void Tick(float time)
        {

        }

        protected GameApp Parent
        {
            get
            {
                return m_root;
            }
        }

        private GameApp m_root;
    }

}