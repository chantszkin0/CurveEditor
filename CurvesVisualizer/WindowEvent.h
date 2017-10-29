//
//  WindowEvent.h
//  CurvesVisualizer
//
//  Created by Chan Tsz Kin on 27/10/2017.
//  Copyright © 2017 Chan Tsz Kin. All rights reserved.
//

#include <string>

#ifndef WindowEvent_h
#define WindowEvent_h

class WindowEvent {
public:
    //  Keyboard
    bool ESCAPE;
    bool NUM1;
    bool PLUS;
    bool MINUS;
    //  Mouse
    bool LEFTMOUSE;
    
    void KeyPressed (std::string key_code)
    {
        if(key_code == "ESCAPE"){
            // Do things once when pressed escape
        }
    }
};


#endif /* WindowEvent_h */
