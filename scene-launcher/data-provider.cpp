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
const int HOUR_24_TO_12 = 12;
} // namespace

namespace SceneLauncher
{
  DataProvider::DataProvider()
  : mPreviousHour( -1 ),
    mPreviousMin( -1 ),
    mPreviousSec( -1 )
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
    for( Dali::Vector<Observer*>::Iterator it = mObservers.Begin(), endIt = mObservers.End(); it != endIt; ++it )
    {
      if( observer == *it )
      {
        // Does not mantain the order.
        mObservers.Remove( it );
        return;
      }
    }
  }

  bool DataProvider::TimerTick()
  {
    timeval tv;
    gettimeofday( &tv, NULL );

    struct tm* currentTime = localtime( &tv.tv_sec );

    const float secondMillisecond = static_cast<float>( currentTime->tm_sec ) + static_cast<float>( tv.tv_usec ) * USEC_TO_SEC;
    const float minuteSecond = static_cast<float>( currentTime->tm_min ) + static_cast<float>( currentTime->tm_sec ) * SEC_TO_MIN;
    const float hour011Minute = static_cast<float>( currentTime->tm_hour % HOUR_24_TO_12 ) + static_cast<float>( currentTime->tm_min ) * MIN_TO_HOUR;
    const float hour023Minute = static_cast<float>( currentTime->tm_hour ) + static_cast<float>( currentTime->tm_min ) * MIN_TO_HOUR;

    const Notification secondMillisecondNotification( Notification::SECOND_MILLISECOND, secondMillisecond );
    const Notification minuteSecondNotification( Notification::MINUTE_SECOND, minuteSecond );
    const Notification hour011MinuteNotification( Notification::HOUR_0_11_MINUTE, hour011Minute );
    const Notification hour023MinuteNotification( Notification::HOUR_0_23_MINUTE, hour023Minute );

    for( Dali::Vector<Observer*>::ConstIterator it =  mObservers.Begin(), endIt = mObservers.End(); it != endIt; ++it )
    {
      Observer* observer = *it;

      observer->OnNotification( secondMillisecondNotification );

      if( ( currentTime->tm_sec != mPreviousSec ) || ( currentTime->tm_min != mPreviousMin ) )
      {
        observer->OnNotification( minuteSecondNotification );
      }

      if( ( currentTime->tm_min != mPreviousMin ) || ( currentTime->tm_hour != mPreviousHour ) )
      {
        observer->OnNotification( hour011MinuteNotification );
        observer->OnNotification( hour023MinuteNotification );
      }
    }

    mPreviousSec = currentTime->tm_sec;
    mPreviousMin = currentTime->tm_min;
    mPreviousHour = currentTime->tm_hour;

    return true;
  }

} // namespace SceneLauncher
