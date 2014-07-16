
using System;
namespace MagicPads
{

    public static class PullJoystick
    {
        public enum EventType
        {
            Unknown,
            Down,
            Up,
            Change,
            Escape,
        }
        public delegate void Event(int id, EventType type, float weight, float dir_x, float dir_y, int pull_x, int pull_y);
    }

    public class PullJoystickTrigger : Trigger
    {
        public static event PullJoystick.Event EventHandle
        {
            add
            {

                _eventHandle += value;
            }
            remove
            {
                _eventHandle -= value;
            }
        }

        private static PullJoystick.Event _eventHandle = null;

        public static Trigger Create(int _origin_x, int _origin_y, float _begin_r, float _end_r, float _escape_r)
        {
            return new PullJoystickTrigger(_origin_x, _origin_y, _begin_r, _end_r, _escape_r);
        }

        protected PullJoystickTrigger(int _origin_x, int _origin_y, float _begin_r, float _end_r, float _escape_r)
        {
            mOriginX = _origin_x;
            mOriginY = _origin_y;
            mBeginR = _begin_r;
            mEndR = _end_r;
            mEscapeR = _escape_r;

            mUnitLength = (_end_r - _begin_r);
            clear();
        }

        protected override bool checkTouch(int touchID, MagicPad.TouchEvent touchEvent, int localX, int localY)
        {
            if (touchID == FocusID)
            {
                return checkTouchEvent(touchID, touchEvent, localX, localY);
            }
            return checkFreeTouchEvent(touchID, touchEvent, localX, localY);
        }

        protected override void reset()
        {
            invokeEvent(PullJoystick.EventType.Escape, 0.0f, 0.0f, 0.0f, 0, 0);
            clear();
            base.reset();
        }

        protected void invokeEvent(PullJoystick.EventType type, float weight, float dir_x, float dir_y, int pull_x, int pull_y)
        {
            if (null != _eventHandle)
            {
                _eventHandle(ID, type, weight, dir_x, dir_y, pull_x, pull_y);
            }

        }



        protected void set(float _weight, int _pull_x, int _pull_y, float _dir_x, float _dir_y)
        {
            mWeight = _weight;
            mPullX = _pull_x;
            mPullY= _pull_y;
            mDirX = _dir_x;
            mDirY = _dir_y;
        }

        protected bool checkTouchEvent(int touchID, MagicPad.TouchEvent touchEvent, int localX, int localY)
        {
            int pull_x = (localX - mOriginX);
            int pull_y = (localY - mOriginY);
            float x = (float)(pull_x);
            float y = (float)(pull_y);

            float r = (float)Math.Sqrt((double)(x * x + y * y));
            if (r > mEscapeR)
            {
                float weight = (r - mBeginR) / mUnitLength;
                float num = 1.0f / r;
                float dir_x = x * num;
                float dir_y = y * num;
                set(weight, pull_x, pull_y, dir_x, dir_y);
                clearFocus();
                invokeEvent(PullJoystick.EventType.Escape, mWeight, mDirX, mDirY, mPullX, mPullY);
                return false;
            }
            else
            {
                if (r < 0.001f)
                {
                    set(0.0f, pull_x, pull_y, 0.0f, 0.0f);
                }
                else
                {
                    float weight = (r - mBeginR) / mUnitLength;
                    float num = 1.0f / r;
                    float dir_x = x * num;
                    float dir_y = y * num;
                    set(weight, pull_x, pull_y, dir_x, dir_y);
                }
                PullJoystick.EventType type = PullJoystick.EventType.Unknown;
                switch (touchEvent)
                {
                    case MagicPad.TouchEvent.TouchMotionDown:
                        {
                            type = PullJoystick.EventType.Down;
                            break;
                        }
  
                    case MagicPad.TouchEvent.TouchMotionMove:
                        {
                            type = PullJoystick.EventType.Change;
                            break;
                        }
                    case MagicPad.TouchEvent.TouchMotionUp:
                        {
                            type = PullJoystick.EventType.Up;
                            clearFocus();
                            break;
                        }
                    case MagicPad.TouchEvent.TouchMotionCancel:
                        {
                            clearFocus();
                            type = PullJoystick.EventType.Up;
                            break;
                        }
                    default:
                        break;
                }
                invokeEvent(type, mWeight, mDirX, mDirY, mPullX, mPullY);
                return true;
            }
        }
        protected bool checkFreeTouchEvent(int touchID, MagicPad.TouchEvent touchEvent, int localX, int localY)
        {
            if (touchEvent == MagicPad.TouchEvent.TouchMotionDown || touchEvent == MagicPad.TouchEvent.TouchMotionMove)
            {
                int pull_x = (localX - mOriginX);
                int pull_y = (localY - mOriginY);
                float x = (float)(pull_x);
                float y = (float)(pull_y);

                float r = (float)Math.Sqrt((double)(x * x + y * y));
                if (r > mEscapeR)
                {
                    return false;
                }
                else if (r > mBeginR)
                {
                    if (r < 0.001f)
                    {
                        set(0.0f, pull_x, pull_y, 0.0f, 0.0f);
                    }
                    else
                    {
                        float weight = (r - mBeginR) / mUnitLength;
                        float num = 1.0f / r;
                        float dir_x = x * num;
                        float dir_y = y * num;
                        set(weight, pull_x, pull_y, dir_x, dir_y);
                    }
                }
                else
                {
                    set(0.0f, pull_x, pull_y, 0.0f, 0.0f);
                }
                invokeEvent(PullJoystick.EventType.Down, mWeight, mDirX, mDirY, mPullX, mPullY);
                return true;
            }
            return false;
        }

        public void clear()
        {
            mPullX = 0;
            mPullY = 0;
            mWeight = 0.0f;
            mDirX = 0.0f;
            mDirY = 0.0f;
            clearFocus();
        }
        private int mPullX;
        private int mPullY;
        private float mDirX;
        private float mDirY;
        private float mWeight;

        //config data
        private int mOriginX;
        private int mOriginY;
        private float mBeginR;
        private float mEndR;
        private float mEscapeR;
        private float mUnitLength;

    }
}