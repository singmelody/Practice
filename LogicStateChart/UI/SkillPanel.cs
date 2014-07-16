using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using ScriptGUI;
using Logic;
using RPGData;
using MagicPads;

namespace ScriptRuntime
{
    public class SkillPanel
    {
        private FString m_windowName = "SkillPanel";
        private FString m_attackBtnName = "Attack";
        private bool m_attackButtonPressed = false;
        private int m_attackButtonID = MagicPad.InvalidID;
        public void Init()
        {
            GUI.RegisterLayout(m_windowName, "Layout/SkillPanel.layout", false, true);

            IntPoint pos = GUI.UIWidget.GetPosition(m_windowName, "_Main");
            IntPoint attackBtnPos = GUI.UIWidget.GetPosition(m_windowName, m_attackBtnName);
            IntSize attackBtnSize = GUI.UIWidget.GetSize(m_windowName, m_attackBtnName);

            int halfwight = attackBtnSize.width / 2;
            int ox = pos.left + attackBtnPos.left + halfwight;
            int oy = pos.top + attackBtnPos.top + halfwight;
            float r = (float)halfwight;
            float er = r * 1.5f;

            m_attackButtonID = ScriptRoot.Root.MagicPad.Add(CircleButtonTrigger.Create(ox, oy, r, er));
            ButtonTrigger.EventHandle += attackButtonMsg;

        }

        public bool AttackButtonPressed
        {
            get
            {
                return m_attackButtonPressed;
            }
        }

        private void attackButtonMsg(int id, Button.EventType type)
        {
            if (id == m_attackButtonID)
            {
                if (type == Button.EventType.Down)
                {
                    m_attackButtonPressed = true;
                }
                else if (type == Button.EventType.Up)
                {
                    m_attackButtonPressed = false;
                }
            }
        }
    };
}