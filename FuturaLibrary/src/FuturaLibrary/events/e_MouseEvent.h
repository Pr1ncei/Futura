#pragma once
#include "pch.h"
#include "e_Event.h"

namespace FuturaLibrary {
    class FT_API MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(float x, float y, float xOffset, float yOffset)
            : m_MouseX(x)
            , m_MouseY(y)
            , m_XOffset(xOffset)
            , m_YOffset(yOffset)
        {
        }
        inline float GetX() const { return m_MouseX; }
        inline float GetY() const { return m_MouseY; }
        inline float GetXOffset() const { return m_XOffset; }
        inline float GetYOffset() const { return m_YOffset; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: X:" << GetX() << ", Y:" << GetY()
                << "; xOffet:" << GetXOffset() << ", yOffset:" << GetYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
            EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_MouseX, m_MouseY;
        float m_XOffset, m_YOffset;
    };

    class FT_API MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_XOffset(xOffset)
            , m_YOffset(yOffset)
        {
        }
        inline float GetXOffset() const { return m_XOffset; }
        inline float GetYOffset() const { return m_YOffset; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
            EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_XOffset, m_YOffset;
    };

    class FT_API MouseButtonEvent : public Event {
    public:
        inline int GetMouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    protected:
        MouseButtonEvent(int button)
            : m_Button(button)
        {
        }
        int m_Button;
    };

    class FT_API MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        MouseButtonPressedEvent(int button)
            : MouseButtonEvent(button)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << GetMouseButton();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class FT_API MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        MouseButtonReleasedEvent(int button)
            : MouseButtonEvent(button)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << GetMouseButton();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
} // namespace VoxelEngine