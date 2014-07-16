using System;
using System.Collections.Generic;
namespace MagicPads
{
    public class MagicPad
    {

        public const int InvalidID = -1;
        public enum TouchEvent
        {
            TouchMotionMove,
            TouchMotionDown,
            TouchMotionUp,
            TouchMotionCancel,
        }

        public void Init(int pointX, int pointY, int width, int height)
        {
            mPointX = pointX;
            mPointY = pointY;
            mWidth = width;
            mHeight = height;
        }

        public bool OnTouchEvent(int touch_id, MagicPad.TouchEvent event_type, int screen_x, int screen_y)
        {
            int local_x = screen_x - mPointX;
            int local_y = screen_y - mPointY;
            if (checkFocusTriggers(touch_id, event_type, local_x, local_y))
            {
                return true;
            }
            if (inside(screen_x, screen_y))
            {
                return checkFreeTriggers(touch_id, event_type, local_x, local_y);
            }
            return false;
        }

        public int Add(Trigger tgr)
        {
            if (tgr.IsFree)
            {
                tgr._Active(IDCounter);
                ++IDCounter;
                mTriggers.Add(tgr);
                return tgr.ID;
            }
            return InvalidID;
        }
        public bool Remove(int id)
        {
            for(int i = 0; i < mTriggers.Count; ++i )
            {
                if (mTriggers[i].ID == id)
                {
                    mTriggers[i]._Reset();
                    mTriggers.RemoveAt(i);
                    return true;
                }
            }
            return false;
        }

        private bool checkFocusTriggers(int touch_id, TouchEvent event_type, int local_x, int local_y)
        {
            foreach (Trigger tgr in mTriggers)
            {
                if (tgr._FocusID == touch_id)
                {
                    if (tgr._OnCheck(touch_id, event_type, local_x, local_y))
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        private bool checkFreeTriggers(int touch_id, TouchEvent event_type, int local_x, int local_y)
        {
            foreach (Trigger tgr in mTriggers)
            {
                if (tgr._FocusID == InvalidID)
                {
                    if (tgr._OnCheck(touch_id, event_type, local_x, local_y))
                    {
                        tgr._SetFocus(touch_id);
                        return true;
                    }
                }
            }
            return false;
        }

        private bool inside(int screen_x, int screen_y)
        {
            return (screen_x > mPointX) && (screen_y > mPointY) && ((screen_x - mPointX) < mWidth) && ((screen_y - mPointY) < mHeight);
        }

        private Trigger getFocusTriggerByTouchID(int touch_id)
        {
            foreach (Trigger tgr in mTriggers)
            {
                if (tgr._FocusID == touch_id)
                {
                    return tgr;
                }
            }
            return null;
        }

        private List<Trigger> mTriggers = new List<Trigger>();
        private int mPointX;
        private int mPointY;
        private int mWidth;
        private int mHeight;

        private static int IDCounter = 0;
    }
}