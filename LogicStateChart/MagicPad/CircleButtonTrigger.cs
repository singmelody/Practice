using System;
namespace MagicPads
{
    public class CircleButtonTrigger : ButtonTrigger
    {
        public static Trigger Create(int originX, int originY, float radius, float escapeRadius)
        {
            return new CircleButtonTrigger(originX, originY, radius, escapeRadius);
        }

        protected CircleButtonTrigger(int originX, int originY, float radius, float escapeRadius)
        {
            mOriginX = originX;
            mOriginY = originY;
            mRadius = radius;
            mEscapeRadius = escapeRadius;
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
            invokeEvent(Button.EventType.Up);
            clearFocus();
            base.reset();
        }

        protected bool checkTouchEvent(int touchID, MagicPad.TouchEvent touchEvent, int localX, int localY)
        {
            float x = (float)(localX - mOriginX);
            float y = (float)(localY - mOriginY);

            float r = (float)Math.Sqrt((double)(x * x + y * y));
            if (r > mEscapeRadius)
            {
                clearFocus();
                invokeEvent(Button.EventType.Up);
                return false;
            }
            else if (MagicPad.TouchEvent.TouchMotionUp == touchEvent || MagicPad.TouchEvent.TouchMotionCancel == touchEvent)
            {
                clearFocus();
                invokeEvent(Button.EventType.Up);
                return true;
            }
            return true;
        }

        protected bool checkFreeTouchEvent (int touchID, MagicPad.TouchEvent touchEvent, int localX, int localY)
		{
            if (MagicPad.TouchEvent.TouchMotionUp == touchEvent || MagicPad.TouchEvent.TouchMotionCancel == touchEvent) 
			{
				return false;
			}
            float x = (float)(localX - mOriginX);
            float y = (float)(localY - mOriginY);

            float r = (float)Math.Sqrt((double)(x * x + y * y));
            if (r < mRadius)
            {
                invokeEvent(Button.EventType.Down);
                return true;
            }
            return false;
        }


        private int mOriginX;
        private int mOriginY;
        private float mRadius;
        private float mEscapeRadius;
    }
}