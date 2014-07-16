using System;
namespace Client.UI
{
    public abstract class UIPanel
    {
        public UIPanel(string name, UISystem parent)
        {
            m_name = name;
            m_parent = parent;
        }
        public virtual void Init()
        {

        }

        public virtual void SetVisible(bool b)
        {

        }

        public UISystem Parent
        {
            get
            {
                return m_parent;
            }
        }

        private string m_name;
        private UISystem m_parent;
    }


}