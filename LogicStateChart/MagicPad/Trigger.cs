

namespace MagicPads
{
    public abstract class Trigger
    {
        protected virtual bool checkTouch(int touchID, MagicPad.TouchEvent touchEvent, int localX, int localY)
        {
            return false;
        }

        protected virtual void reset()
        {

        }


        public bool IsFree
        {
            get
            {
                return _id == MagicPad.InvalidID;
            }
        }

        internal int ID
        {
            get
            {
                return _id;
            }
        }

        internal bool IsActived
        {
            get
            {
                return _active;
            }
        }


        protected int FocusID
        {
            get
            {
                return _focus_id;
            }
        }
        protected void clearFocus()
        {
            _focus_id = MagicPad.InvalidID;
        }


        internal bool _OnCheck(int touchID, MagicPad.TouchEvent touchEvent, int localX, int localY)
        {
            return checkTouch(touchID, touchEvent, localX, localY);
        }

        internal void _Active(int id)// just used for MagicPad
        {
            _id = id;
            _active = true;
        }

        internal void _Reset()// just used for MagicPad
        {
            reset();
            _active = false;
            _id = MagicPad.InvalidID;
        }

        internal int _FocusID // just used for MagicPad
        {
            get
            {
                return _focus_id;
            }
        }

        internal void _SetFocus(int id)// just used for MagicPad
        {
            _focus_id = id;
        }

        private int _id = MagicPad.InvalidID;
        private int _focus_id = MagicPad.InvalidID;
        private bool _active = false;
        
    }
}