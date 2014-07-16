
namespace MagicPads
{
    public static class Button
    {
        public enum EventType
        {
            Unknown,
            Down,
            Up,
        }
        public delegate void Event(int id, EventType type);
    }

    public abstract class ButtonTrigger : Trigger
    {
        public static event Button.Event EventHandle
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

        private static Button.Event _eventHandle = null;

        protected void invokeEvent(Button.EventType type)
        {
            if (null != _eventHandle)
            {
                _eventHandle(ID, type);
            }

        }

        private static void invoke(int id)
        {

        }
    }
}