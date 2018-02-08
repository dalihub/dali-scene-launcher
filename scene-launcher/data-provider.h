#ifndef DALI_SCENE_LAUNCHER_DATA_PROVIDER_H
#define DALI_SCENE_LAUNCHER_DATA_PROVIDER_H

/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// EXTERNAL INCLUDES
#include <dali/public-api/adaptor-framework/timer.h>
#include <dali/public-api/common/dali-vector.h>
#include <dali/public-api/signals/connection-tracker.h>

namespace SceneLauncher
{

class DataProvider : public Dali::ConnectionTracker
{
public:
  struct Notification
  {
    enum Type
    {
      SECOND_MILLISECOND,
      MINUTE_SECOND,
      HOUR_0_11_MINUTE,
      HOUR_0_23_MINUTE
    };

    Notification( Type type, float value )
    : mType( type ),
      mValue( value )
    {}

    ~Notification() {};

    Type mType;
    float mValue;
  };

  class Observer
  {
  public:
    virtual ~Observer() {}

    virtual void OnNotification( const Notification& notification ) = 0;
  };

public:
  DataProvider();
  ~DataProvider();

  void Start();
  void Stop();

  void Register( Observer* observer );
  void UnRegister( Observer* observer );

private:
  bool TimerTick();

private:
  Dali::Vector<Observer*> mObservers;
  Dali::Timer mTimer;

  int mPreviousHour;
  int mPreviousMin;
  int mPreviousSec;
};

} // namespace SceneLauncher

#endif // DALI_SCENE_LAUNCHER_DATA_PROVIDER_H
