using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using ScriptGUI;
using Logic;
using RPGData;
using MagicPads;

namespace UserDefGUI
{
    public class JoyStick
    {
        private FString m_windowName = "JoyStick";
        private FString m_joystickBtnName = "JoystickBtn";
        private bool m_isCtrl = false;
        private IntPoint m_jsBtnDefaultPos = new IntPoint(0, 0);
        private int m_joystickID = MagicPad.InvalidID;
        private float m_joystickBeginR;
        private float m_joystickEndR;
        private float m_joystickEscapeR;
        private IntPoint m_joystickDefualtPoint = new IntPoint(0, 0);
        private int m_joystickHalfR;
        private int m_joystickOriginX;
        private int m_joystickOriginY;
        private Vector2 m_joystickDir = Vector2.Zero;
        //private const int MAX_JOYSTICK_MOVE_RADIUS = 50;
        //public const int MIN_JOYSTICK_MOVE_RADIUS = 5;
        public void Init()
        {
            GUI.RegisterLayout(m_windowName, "Layout/JoyStickBtn.layout", false, true);
            //GUI.UIWidget.SetEventMouseButtonPressed(m_windowName, "JoystickBtn", OnJSBtnPressed, EventControl.Add);
            //GUI.UIWidget.SetEventMouseButtonReleased(m_windowName, "JoystickBtn", OnJSBtnReleased, EventControl.Add);
            //GUI.UIWidget.SetEventMouseDrag(m_windowName, "JoystickBtn", OnJSBtnDrag, EventControl.Add);
            //GUI.UIWidget.SetEventTick(m_windowName, "JoystickBtn", Tick, EventControl.Add);
            //m_jsBtnDefaultPos = GUI.UIWidget.GetPosition(m_windowName, "JoystickBtn");

            
            IntPoint pos = GUI.UIWidget.GetPosition(m_windowName, "_Main");
            IntSize size = GUI.UIWidget.GetSize(m_windowName, "_Main");

            m_joystickDefualtPoint = GUI.UIWidget.GetPosition(m_windowName, m_joystickBtnName);
            IntSize joySize = GUI.UIWidget.GetSize(m_windowName, m_joystickBtnName);
            m_joystickHalfR = joySize.width / 2;

            m_joystickOriginX = m_joystickDefualtPoint.left + m_joystickHalfR;
            m_joystickOriginY = m_joystickDefualtPoint.top + m_joystickHalfR;

			float r = ((float)size.width) * 0.5f;
            m_joystickEndR = r * 0.5f;
            m_joystickEscapeR = r * 1.5f;
            m_joystickBeginR = r * 0.1f;

            int ox = m_joystickOriginX + pos.left;
            int oy = m_joystickOriginY + pos.top;
            m_joystickID = ScriptRoot.Root.MagicPad.Add(PullJoystickTrigger.Create(ox, oy, m_joystickBeginR, m_joystickEndR, m_joystickEscapeR));
            PullJoystickTrigger.EventHandle += pullJoystickMsg;
        }
        private void pullJoystickMsg(int id, PullJoystick.EventType type, float weight, float dir_x, float dir_y, int pull_x, int pull_y)
        {
            if (id == m_joystickID)
            {
                if (PullJoystick.EventType.Escape == type || PullJoystick.EventType.Up == type || weight < 0.5f)
                {
                    GUI.UIWidget.SetPosition(m_windowName, m_joystickBtnName, m_joystickDefualtPoint);

                    Message msg = new Message();
                    msg.state = PlayerIdleState.Instance;
                    MessageDispatcher.Instance.DispatchMessage(null, SceneMgr.Instance.player, msg);

                    m_isCtrl = false;
                    m_joystickDir = Vector2.Zero;
                }
                else
                {
                    IntPoint point = new IntPoint();
                    point.left = m_joystickOriginX + (int)(m_joystickEndR * dir_x) - m_joystickHalfR;
                    point.top = m_joystickOriginY + (int)(m_joystickEndR * dir_y) - m_joystickHalfR;
                    GUI.UIWidget.SetPosition(m_windowName, m_joystickBtnName, point);

                    Message msg = new Message();
                    msg.state = PlayerRunState.Instance;
                    MessageDispatcher.Instance.DispatchMessage( null, SceneMgr.Instance.player, msg);
                    
                    
                    m_joystickDir.X = dir_x;
                    m_joystickDir.Y = dir_y;
                    m_isCtrl = true;
                }

            }
        }
        //private void OnJSBtnDrag(FString sender, int value, int value2, MouseButton btn)
        //{
        //    //屏幕坐标系转换成摇杆坐标系
        //    IntPoint panelPosition = GUI.UIWidget.GetPosition(m_windowName, "_Main");
        //    value -= panelPosition.left;
        //    value2 -= panelPosition.top;

        //    //拖拽点与摇杆按钮默认中心点的偏移
        //    int xOffset = value - JoyStickBtnCentreDefaultPos.left;
        //    int yOffset = value2 - JoyStickBtnCentreDefaultPos.top;
        //    int xyOffset = (int)Math.Sqrt(xOffset * xOffset + yOffset * yOffset);

        //    if (MIN_JOYSTICK_MOVE_RADIUS < xyOffset)
        //    {
        //        int xCurrOffset = xOffset * MAX_JOYSTICK_MOVE_RADIUS / xyOffset;
        //        int yCurrOffset = yOffset * MAX_JOYSTICK_MOVE_RADIUS / xyOffset;

        //        IntPoint joystickPosition = new IntPoint(xCurrOffset + JoyStickBtnDefaultPos.left, yCurrOffset + JoyStickBtnDefaultPos.top);
        //        GUI.UIWidget.SetPosition(m_windowName, "JoystickBtn", joystickPosition);
        //        //TODO AutoMove
        //        //MoveMgr.Instance.StopAutoMove(Player.Instance.Data);
        //        Player.Instance.Data.AvatarState = CharacterState.Running;
        //    }
        //}

        //private void OnJSBtnPressed(FString sender, int value, int value2, MouseButton btn)
        //{
        //    m_isCtrl = true;
        //    Debug.Dbgout("isCtrl");
        //}

        //private void OnJSBtnReleased(FString sender, int value, int value2, MouseButton btn)
        //{
        //    m_isCtrl = false;
        //    Debug.Dbgout("Released");
        //}

        //private void Tick(FString sender, float gameTime, float frameTickTime)
        //{
        //    if (!m_isCtrl)
        //    {
        //        GUI.UIWidget.SetPosition(m_windowName, "JoystickBtn", JoyStickBtnDefaultPos);
        //        if (CharacterState.Running == Player.Instance.Data.AvatarState && !Player.Instance.Data.IsAutoMove)
        //        {
        //            Player.Instance.Data.AvatarState = CharacterState.Idle;
        //        }
        //    }
        //}

        public bool IsCtrl
        {
            get
            {
                return m_isCtrl;
            }
        }

        public Vector2 JoystickDir
        {
            get
            {
                return m_joystickDir;
            }
        }

        public void SetVisable(bool vis)
        {
            GUI.SetLayoutVisible(m_windowName, vis);
        }

    };
}