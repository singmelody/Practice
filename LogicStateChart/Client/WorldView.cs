using System;
using ScriptRuntime;
namespace Client
{
    public class WorldView
    {

        public int ScreenWidth
        {
            get
            {
                return m_screenWidth;
            }
        }

        public int ScreenHeight
        {
            get
            {
                return m_screenHeight;
            }
        }

        public int UIWidth
        {
            get
            {
                return m_UIWidth;
            }
        }

        public int UIHeight
        {
            get
            {
                return m_UIHeight;
            }
        }

        public void Init()
        {
            m_screenWidth = GraphicSystem.GetWidth();
            m_screenHeight = GraphicSystem.GetHeight();

            m_UIWidth = m_screenWidth;
            m_UIHeight = m_screenHeight;
        }
        

        private int m_screenWidth;
        private int m_screenHeight;
        private int m_UIWidth;
        private int m_UIHeight;
        
    }
}