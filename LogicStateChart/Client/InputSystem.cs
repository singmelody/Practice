using System;
using ScriptRuntime;
using MagicPads;

namespace Client
{
    public class InputSystem : GameSystem
    {
        public InputSystem(GameApp app)
            :base(app)
        {

        }

        public void SetPullJoyStick(int _origin_x, int _origin_y, float _begin_r, float _end_r, float _escape_r)
        {
            m_joystickID = ScriptRoot.Root.MagicPad.Add(PullJoystickTrigger.Create(_origin_x, _origin_y, _begin_r, _end_r, _escape_r));
        }

        public override void Init()
        {
            m_magicPad.Init(0, 0, Parent.WorldView.ScreenWidth, Parent.WorldView.ScreenHeight);
            PullJoystickTrigger.EventHandle += pullJoystickMsg;
            ButtonTrigger.EventHandle += buttonMsg;

            base.Init();
        }


        public override void Tick(float time)
        {
            base.Tick(time);
        }


        private void pullJoystickMsg(int id, PullJoystick.EventType type, float weight, float dir_x, float dir_y, int pull_x, int pull_y)
        {

        }

        private void buttonMsg(int id, Button.EventType type)
        {

        }

        private void upateMagicPad()
        {
            TouchEvent[] tes = Input.GetCurrentTouchEvents();
            foreach (TouchEvent te in tes)
            {
                Vector2 pos = Input.GetScreenTouchPixelPosition(te.id);
                switch (te.eventType)
                {
                    case InputEventType.TouchMotionDown:
                        {
                            ScriptRoot.Root.MagicPad.OnTouchEvent(te.id,
                                MagicPad.TouchEvent.TouchMotionDown,
                                (int)pos.X, (int)pos.Y);
                            break;
                        }
                    case InputEventType.TouchMotionUp:
                        {
                            ScriptRoot.Root.MagicPad.OnTouchEvent(te.id,
                                MagicPad.TouchEvent.TouchMotionUp,
                                (int)pos.X, (int)pos.Y);
                            break;
                        }
                    case InputEventType.TouchMotionCancel:
                        {
                            ScriptRoot.Root.MagicPad.OnTouchEvent(te.id,
                                MagicPad.TouchEvent.TouchMotionCancel,
                                (int)pos.X, (int)pos.Y);
                            break;
                        }
                    case InputEventType.TouchMotionMove:
                        {
                            ScriptRoot.Root.MagicPad.OnTouchEvent(te.id,
                                MagicPad.TouchEvent.TouchMotionMove,
                                (int)pos.X, (int)pos.Y);

                            break;
                        }
                    default:
                        break;
                }
            }
            MouseEvent[] mes = Input.GetCurrentMouseEvents();
            if (mes.Length > 0)
            {
                Vector2 pos = Vector2.Zero;
                Input.GetScreenPosition(out pos);
                pos.X *= Parent.WorldView.ScreenWidth;
                pos.X *= Parent.WorldView.ScreenHeight;
                foreach (MouseEvent me in mes)
                {
                    if ((me.eventType == InputEventType.MouseMove || me.button == MouseCode.LeftButton) && Input.MouseButtonPressed(MouseCode.LeftButton))
                    {
                        switch (me.eventType)
                        {
                            case InputEventType.MouseButtonDown:
                                {
                                    ScriptRoot.Root.MagicPad.OnTouchEvent(MouseID,
                                        MagicPad.TouchEvent.TouchMotionDown,
                                        (int)pos.X, (int)pos.Y);
                                    break;
                                }
                            case InputEventType.MouseButtonUp:
                                {
                                    ScriptRoot.Root.MagicPad.OnTouchEvent(MouseID,
                                        MagicPad.TouchEvent.TouchMotionUp,
                                        (int)pos.X, (int)pos.Y);
                                    break;
                                }
                            case InputEventType.MouseMove:
                                {
                                    if (!Input.MouseButtonUp(MouseCode.LeftButton))
                                    {
                                        ScriptRoot.Root.MagicPad.OnTouchEvent(MouseID,
                                            MagicPad.TouchEvent.TouchMotionMove,
                                            (int)pos.X, (int)pos.Y);
                                    }

                                    break;
                                }
                            default:
                                break;
                        }
                    }
                }
            }

        }

        private MagicPad m_magicPad = new MagicPad();
        private const int MouseID = 100000;
        private int m_joystickID = MagicPad.InvalidID;
    }
}