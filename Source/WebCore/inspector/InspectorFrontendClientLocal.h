/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef InspectorFrontendClientLocal_h
#define InspectorFrontendClientLocal_h

#include "InspectorFrontendClient.h"
#include <wtf/Forward.h>
#include <wtf/Noncopyable.h>
#include <wtf/text/WTFString.h>

namespace WebCore {

class Frame;
class InspectorController;
class InspectorBackendDispatchTask;
class InspectorFrontendHost;
class Page;

class InspectorFrontendClientLocal : public InspectorFrontendClient {
    WTF_MAKE_NONCOPYABLE(InspectorFrontendClientLocal); WTF_MAKE_FAST_ALLOCATED;
public:
    class Settings {
    public:
        Settings() { }
        virtual ~Settings() { }
        virtual String getProperty(const String& name);
        virtual void setProperty(const String& name, const String& value);
    };

    WEBCORE_EXPORT InspectorFrontendClientLocal(InspectorController*, Page*, std::unique_ptr<Settings>);
    WEBCORE_EXPORT virtual ~InspectorFrontendClientLocal();

    WEBCORE_EXPORT virtual void windowObjectCleared() override;
    WEBCORE_EXPORT virtual void frontendLoaded() override;

    WEBCORE_EXPORT virtual void moveWindowBy(float x, float y) override;

    WEBCORE_EXPORT virtual void requestSetDockSide(DockSide) override;
    WEBCORE_EXPORT virtual void changeAttachedWindowHeight(unsigned) override;
    WEBCORE_EXPORT virtual void changeAttachedWindowWidth(unsigned) override;
    WEBCORE_EXPORT virtual void openInNewTab(const String& url) override;
    virtual bool canSave()  override { return false; }
    virtual void save(const String&, const String&, bool, bool) override { }
    virtual void append(const String&, const String&) override { }

    virtual void attachWindow(DockSide) = 0;
    virtual void detachWindow() = 0;

    WEBCORE_EXPORT virtual void sendMessageToBackend(const String& message) override;

    WEBCORE_EXPORT virtual bool isUnderTest() override;

    WEBCORE_EXPORT bool canAttachWindow();
    WEBCORE_EXPORT void setDockingUnavailable(bool);

    WEBCORE_EXPORT static unsigned constrainedAttachedWindowHeight(unsigned preferredHeight, unsigned totalWindowHeight);
    WEBCORE_EXPORT static unsigned constrainedAttachedWindowWidth(unsigned preferredWidth, unsigned totalWindowWidth);

    // Direct Frontend API
    WEBCORE_EXPORT bool isDebuggingEnabled();
    WEBCORE_EXPORT void setDebuggingEnabled(bool);

    WEBCORE_EXPORT bool isTimelineProfilingEnabled();
    WEBCORE_EXPORT void setTimelineProfilingEnabled(bool);

    WEBCORE_EXPORT bool isProfilingJavaScript();
    WEBCORE_EXPORT void startProfilingJavaScript();
    WEBCORE_EXPORT void stopProfilingJavaScript();

    WEBCORE_EXPORT void showConsole();

    WEBCORE_EXPORT void showMainResourceForFrame(Frame*);

    WEBCORE_EXPORT void showResources();

    WEBCORE_EXPORT void setAttachedWindow(DockSide);

protected:
    virtual void setAttachedWindowHeight(unsigned) = 0;
    virtual void setAttachedWindowWidth(unsigned) = 0;
    WEBCORE_EXPORT void restoreAttachedWindowHeight();

private:
    bool evaluateAsBoolean(const String& expression);
    void evaluateOnLoad(const String& expression);

    friend class FrontendMenuProvider;
    InspectorController* m_inspectorController;
    Page* m_frontendPage;
    // TODO(yurys): this ref shouldn't be needed.
    RefPtr<InspectorFrontendHost> m_frontendHost;
    std::unique_ptr<InspectorFrontendClientLocal::Settings> m_settings;
    bool m_frontendLoaded;
    DockSide m_dockSide;
    Vector<String> m_evaluateOnLoad;
    std::unique_ptr<InspectorBackendDispatchTask> m_dispatchTask;
};

} // namespace WebCore

#endif
