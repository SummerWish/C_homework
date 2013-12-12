//
//  MyTimer.h
//  1352978
//
//  Created by Breezewish on 13-12-12.
//  Copyright (c) 2013年 Breezewish. All rights reserved.
//

#ifndef _352978_MyTimer_h
#define _352978_MyTimer_h

#include <sys/time.h>
#include <unistd.h>

//http://stackoverflow.com/questions/588307/c-obtaining-milliseconds-time-on-linux-clock-doesnt-seem-to-work-properl

class MyTimer
{
private:
    struct timeval start;
public:
    MyTimer()
    {
        gettimeofday(&start, NULL);
    }
    
    long elapsed()
    {
        struct timeval end;
        gettimeofday(&end, NULL);
        
        long mtime, seconds, useconds;
        
        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        
        mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
        
        return mtime;
    }
};

#endif
