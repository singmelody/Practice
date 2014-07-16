using System;
using ScriptRuntime;
using ScriptGUI;

namespace Client.UI
{
    public class JoyStickPanel : G3dUIPanel
    {
        public JoyStickPanel(string name, UISystem parent)
            :base(name, parent)
        {

        }

        private FString m_joystickBtnName = "JoystickBtn";
        private IntPoint m_jsBtnDefaultPos = new IntPoint(0, 0);
        private float m_joystickBeginR;
        private float m_joystickEndR;
        private float m_joystickEscapeR;
        private IntPoint m_joystickDefualtPoint = new IntPoint(0, 0);
        private int m_joystickHalfR;
        private int m_joystickOriginX;
        private int m_joystickOriginY;

        public override void Init()
        {
            GUI.RegisterLayout(KeyName, "Layout/JoyStickBtn.layout", false, true);

            IntPoint pos = GUI.UIWidget.GetPosition(KeyName, "_Main");
            IntSize size = GUI.UIWidget.GetSize(KeyName, "_Main");

            m_joystickDefualtPoint = GUI.UIWidget.GetPosition(KeyName, m_joystickBtnName);
            IntSize joySize = GUI.UIWidget.GetSize(KeyName, m_joystickBtnName);
            m_joystickHalfR = joySize.width / 2;

            m_joystickOriginX = m_joystickDefualtPoint.left + m_joystickHalfR;
            m_joystickOriginY = m_joystickDefualtPoint.top + m_joystickHalfR;

            float r = ((float)size.width) * 0.5f;
            m_joystickEndR = r * 0.5f;
            m_joystickEscapeR = r * 1.5f;
            m_joystickBeginR = r * 0.1f;

            int ox = m_joystickOriginX + pos.left;
            int oy = m_joystickOriginY + pos.top;
        }

        public override void SetVisible(bool b)
        {
            GUI.SetLayoutVisible(KeyName, b);
        }

    };


}