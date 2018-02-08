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

// CLASS HEADER
#include "data-provider.h"

// EXTERNAL INCLUDES
#include <sys/time.h>

namespace
{
const float MIN_TO_HOUR = 1.f / 60.f;
const float SEC_TO_MIN = 1.f / 60.f;
const float USEC_TO_SEC = 1.f / 1000000.f;
} // namespace

namespace SceneLauncher
{
  DataProvider::DataProvider()
  {
    mTimer = Dali::Timer::New( 200u );

    mTimer.TickSignal().Connect( this, &DataProvider::TimerTick );
  }

  DataProvider::~DataProvider()
  {
  }

  void DataProvider::Start()
  {
    mTimer.Start();
  }

  void DataProvider::Stop()
  {
    mTimer.Stop();
  }

  void DataProvider::Register( Observer* observer )
  {
    mObservers.PushBack( observer );
  }

  void DataProvider::UnRegister( Observer* observer )
  {
    // TODO
  }

  bool DataProvider::TimerTick()
  {
    timeval tv;
    gettimeofday( &tv, NULL );

    struct tm* currentTime = localtime( &tv.tv_sec );

    const float secondMillisecond = static_cast<float>( currentTime->tm_sec ) + static_cast<float>( tv.tv_usec ) * USEC_TO_SEC;
    const float minuteSecond = static_cast<float>( currentTime->tm_min ) + static_cast<float>( currentTime->tm_sec ) * SEC_TO_MIN;
    const float hour011Minute = static_cast<float>( currentTime->tm_hour ) + static_cast<float>( currentTime->tm_min ) * MIN_TO_HOUR;

    const Notification secondMillisecondNotification( Notification::SECOND_MILLISECOND, secondMillisecond );
    const Notification minuteSecondNotification( Notification::MINUTE_SECOND, minuteSecond );
    const Notification hour011MinuteNotification( Notification::HOUR_0_11_MINUTE, hour011Minute );
    const Notification hour023MinuteNotification( Notification::HOUR_0_23_MINUTE, hour011Minute );

    for( Dali::Vector<Observer*>::ConstIterator it =  mObservers.Begin(), endIt = mObservers.End(); it != endIt; ++it )
    {
      Observer* observer = *it;

      observer->Notify( secondMillisecondNotification );
      observer->Notify( minuteSecondNotification );
      observer->Notify( hour011MinuteNotification );
      observer->Notify( hour023MinuteNotification );
    }

    return true;
  }

} // namespace SceneLauncher
