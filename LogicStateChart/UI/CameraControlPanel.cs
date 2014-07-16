using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;
using ScriptGUI;
using Logic;
using RPGData;
using MagicPads;

namespace ScriptRuntime
{
    public class CameraControlPanel
    {
        private FString m_windowName = "CameraControl";
        private int m_LeftBtnID = MagicPad.InvalidID;
        private int m_RightBtnID = MagicPad.InvalidID;
        public void Init()
        {
            GUI.RegisterLayout(m_windowName, "Layout/CameraControl.layout", false, true);

            //GUI.UIWidget.SetEventMouseButtonPressed(m_windowName, "LeftCR", OnLeftCRBtnPressed, EventControl.Add);
            //GUI.UIWidget.SetEventMouseButtonReleased(m_windowName, "LeftCR", OnLeftCRBtnReleased, EventControl.Add);

            //GUI.UIWidget.SetEventMouseButtonPressed(m_windowName, "RightCR", OnRightCRBtnPressed, EventControl.Add);
            //GUI.UIWidget.SetEventMouseButtonReleased(m_windowName, "RightCR", OnRightCRBtnReleased, EventControl.Add);

             IntPoint pos = GUI.UIWidget.GetPosition(m_windowName, "_Main");

            {

                IntPoint attackBtnPos = GUI.UIWidget.GetPosition(m_windowName, "LeftCR");
                IntSize attackBtnSize = GUI.UIWidget.GetSize(m_windowName, "LeftCR");

                int halfwight = attackBtnSize.width / 2;
                int ox = pos.left + attackBtnPos.left + halfwight;
                int oy = pos.top + attackBtnPos.top + halfwight;
                float r = (float)halfwight;
                float er = r * 1.5f;

                m_LeftBtnID = ScriptRoot.Root.MagicPad.Add(CircleButtonTrigger.Create(ox, oy, r, er));

            }

            {
                IntPoint attackBtnPos = GUI.UIWidget.GetPosition(m_windowName, "RightCR");
                IntSize attackBtnSize = GUI.UIWidget.GetSize(m_windowName, "RightCR");

                int halfwight = attackBtnSize.width / 2;
                int ox = pos.left + attackBtnPos.left + halfwight;
                int oy = pos.top + attackBtnPos.top + halfwight;
                float r = (float)halfwight;
                float er = r * 1.1f;
                m_RightBtnID = ScriptRoot.Root.MagicPad.Add(CircleButtonTrigger.Create(ox, oy, r, er));
            }
            ButtonTrigger.EventHandle += attackButtonMsg;
        }

        private void attackButtonMsg(int id, Button.EventType type)
        {
            if (type == Button.EventType.Down)
            {
                if (id == m_LeftBtnID)
                {
                    CameraMgr.Instance.State = CameraState.eRotateLeft;
                }
                else if (id == m_RightBtnID)
                {
                    CameraMgr.Instance.State = CameraState.eRotateRight;
                }
            }
            else if (type == Button.EventType.Up)
            {
                CameraMgr.Instance.State = CameraState.eNone;
            }
        }


        //private void OnLeftCRBtnPressed(FString sender, int value, int value2, MouseButton btn)
        //{
        //    CameraMgr.Instance.State = CameraState.eRotateLeft;
        //}

        //private void OnLeftCRBtnReleased(FString sender, int value, int value2, MouseButton btn)
        //{
        //    CameraMgr.Instance.State = CameraState.eNone;
        //}

        //private void OnRightCRBtnPressed(FString sender, int value, int value2, MouseButton btn)
        //{
        //    CameraMgr.Instance.State = CameraState.eRotateRight;
        //}

        //private void OnRightCRBtnReleased(FString sender, int value, int value2, MouseButton btn)
        //{
        //    CameraMgr.Instance.State = CameraState.eNone;
        //}
    };
}