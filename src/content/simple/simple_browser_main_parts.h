// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_SIMPLE_BROWSER_MAIN_PARTS_H_
#define CONTENT_SIMPLE_SIMPLE_BROWSER_MAIN_PARTS_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/browser_main_parts.h"

class CMainFrame;
namespace content {

struct MainFunctionParams;
class SimpleBrowserContext;


// This class contains different "stages" to be executed by |BrowserMain()|,
// Each stage is represented by a single BrowserMainParts method, called from
// the corresponding method in |BrowserMainLoop| (e.g., EarlyInitialization())
// which does the following:
//  - calls a method (e.g., "PreEarlyInitialization()") which implements
//    platform / tookit specific code for that stage.
//  - calls various methods for things common to all platforms (for that stage).
//  - calls a method (e.g., "PostEarlyInitialization()") for platform-specific
//    code to be called after the common code.
//
// Stages:
//  - EarlyInitialization: things which should be done as soon as possible on
//    program start (such as setting up signal handlers) and things to be done
//    at some generic time before the start of the main message loop.
//  - MainMessageLoopStart: things beginning with the start of the main message
//    loop and ending with initialization of the main thread; platform-specific
//    things which should be done immediately before the start of the main
//    message loop should go in |PreMainMessageLoopStart()|.
//  - RunMainMessageLoopParts:  things to be done before and after invoking the
//    main message loop run method (e.g. MessageLoopForUI::current()->Run()).
//
// How to add stuff (to existing parts):
//  - Figure out when your new code should be executed. What must happen
//    before/after your code is executed? Are there performance reasons for
//    running your code at a particular time? Document these things!
//  - Split out any platform-specific bits. Please avoid #ifdefs it at all
//    possible. You have two choices for platform-specific code: (1) Execute it
//    from one of the platform-specific |Pre/Post...()| methods; do this if the
//    code is unique to a platform type. Or (2) execute it from one of the
//    "parts" (e.g., |EarlyInitialization()|) and provide platform-specific
//    implementations of your code (in a virtual method); do this if you need to
//    provide different implementations across most/all platforms.
//  - Unless your new code is just one or two lines, put it into a separate
//    method with a well-defined purpose. (Likewise, if you're adding to an
//    existing chunk which makes it longer than one or two lines, please move
//    the code out into a separate method.)
//
// 这个类被BrowserMain在不同阶段调用
class SimpleBrowserMainParts : public BrowserMainParts {
 public:
  explicit SimpleBrowserMainParts(const MainFunctionParams& parameters);
  virtual ~SimpleBrowserMainParts();

  // BrowserMainParts overrides.
  virtual void PreMainMessageLoopRun() OVERRIDE;



  SimpleBrowserContext* browser_context() { return browser_context_.get(); }
  SimpleBrowserContext* off_the_record_browser_context() {
    return off_the_record_browser_context_.get();
  }

private:
public:
  scoped_ptr<SimpleBrowserContext> browser_context_;
  scoped_ptr<SimpleBrowserContext> off_the_record_browser_context_;
  scoped_ptr<CMainFrame> main_ui_;

  // For running content_browsertests.
  const MainFunctionParams& parameters_;
  bool run_message_loop_;

  DISALLOW_COPY_AND_ASSIGN(SimpleBrowserMainParts);
};

}  // namespace content

#endif  // CONTENT_SIMPLE_SIMPLE_BROWSER_MAIN_PARTS_H_
