#pragma once



namespace nui
{
    namespace Ui
    {
        class EventType
        {
        public:
            enum Event
            {
                // Key
                Event_KeyDown,
                Event_KeyUp,
                Event_KeyPress,

                // Mouse
                Event_MouseMove,

                Event_MouseDown,
                Event_MouseUp,
                Event_Click,
                Event_DoubleClick,

                Event_RightMouseDown,
                Event_RightMouseUp,
                Event_RightClick,
                Event_RightDoubleClick,

                Event_MouseHover,
                Event_MouseLeave,

                // Paint
                Event_Paint,

                // 
            };
        };
    }
}