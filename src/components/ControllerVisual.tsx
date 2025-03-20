
import { useEffect, useRef } from 'react';
import { useController } from '@/context/ControllerContext';
import { cn } from '@/lib/utils';

export function ControllerVisual() {
  const { controllerState, isEmulationActive } = useController();
  const { buttons, leftStick, rightStick } = controllerState;
  
  // References for the stick positions
  const leftStickRef = useRef<HTMLDivElement>(null);
  const rightStickRef = useRef<HTMLDivElement>(null);
  
  // Update stick positions
  useEffect(() => {
    if (!leftStickRef.current || !rightStickRef.current) return;
    
    const leftStickContainer = leftStickRef.current.parentElement;
    const rightStickContainer = rightStickRef.current.parentElement;
    
    if (!leftStickContainer || !rightStickContainer) return;
    
    // Get center positions
    const leftCenterX = leftStickContainer.clientWidth / 2;
    const leftCenterY = leftStickContainer.clientHeight / 2;
    const rightCenterX = rightStickContainer.clientWidth / 2;
    const rightCenterY = rightStickContainer.clientHeight / 2;
    
    // Calculate max distance (radius * 0.6)
    const leftMaxDistance = leftStickContainer.clientWidth * 0.3;
    const rightMaxDistance = rightStickContainer.clientWidth * 0.3;
    
    // Update left stick position
    leftStickRef.current.style.left = `${leftCenterX + leftStick.x * leftMaxDistance}px`;
    leftStickRef.current.style.top = `${leftCenterY + leftStick.y * leftMaxDistance}px`;
    
    // Update right stick position
    rightStickRef.current.style.left = `${rightCenterX + rightStick.x * rightMaxDistance}px`;
    rightStickRef.current.style.top = `${rightCenterY + rightStick.y * rightMaxDistance}px`;
  }, [leftStick, rightStick]);

  return (
    <div className={cn(
      "glass-panel p-8 w-full max-w-2xl mx-auto transition-opacity duration-300",
      isEmulationActive ? "opacity-100" : "opacity-70"
    )}>
      <div className="grid grid-cols-2 gap-10">
        {/* Left side - Analog sticks */}
        <div className="space-y-8">
          <div className="text-center">
            <span className="text-xs uppercase tracking-wider text-muted-foreground">Left Analog</span>
            <div id="left-stick" className="analog-stick mx-auto mt-2">
              <div 
                ref={leftStickRef}
                id="left-stick-knob" 
                className="analog-knob left-1/2 top-1/2"
              ></div>
            </div>
          </div>
          
          <div className="text-center">
            <span className="text-xs uppercase tracking-wider text-muted-foreground">Right Analog</span>
            <div id="right-stick" className="analog-stick mx-auto mt-2">
              <div 
                ref={rightStickRef}
                id="right-stick-knob" 
                className="analog-knob left-1/2 top-1/2"
              ></div>
            </div>
          </div>
        </div>
        
        {/* Right side - Buttons */}
        <div className="flex flex-col justify-between">
          {/* Face buttons */}
          <div className="relative h-36 w-36 mx-auto">
            <div 
              id="button-Y" 
              className={cn(
                "controller-button absolute top-0 left-1/2 transform -translate-x-1/2",
                buttons.Y && "controller-button-active"
              )}
            >
              Y
            </div>
            <div 
              id="button-X" 
              className={cn(
                "controller-button absolute left-0 top-1/2 transform -translate-y-1/2",
                buttons.X && "controller-button-active"
              )}
            >
              X
            </div>
            <div 
              id="button-B" 
              className={cn(
                "controller-button absolute right-0 top-1/2 transform -translate-y-1/2",
                buttons.B && "controller-button-active"
              )}
            >
              B
            </div>
            <div 
              id="button-A" 
              className={cn(
                "controller-button absolute bottom-0 left-1/2 transform -translate-x-1/2",
                buttons.A && "controller-button-active"
              )}
            >
              A
            </div>
          </div>
          
          {/* Shoulder buttons */}
          <div className="flex justify-between mt-6">
            <div>
              <div 
                id="button-LB" 
                className={cn(
                  "controller-button w-20 h-10 mb-2",
                  buttons.LB && "controller-button-active"
                )}
              >
                LB
              </div>
              <div 
                id="button-LT" 
                className={cn(
                  "controller-button w-20 h-10",
                  buttons.LT && "controller-button-active"
                )}
              >
                LT
              </div>
            </div>
            <div>
              <div 
                id="button-RB" 
                className={cn(
                  "controller-button w-20 h-10 mb-2",
                  buttons.RB && "controller-button-active"
                )}
              >
                RB
              </div>
              <div 
                id="button-RT" 
                className={cn(
                  "controller-button w-20 h-10",
                  buttons.RT && "controller-button-active"
                )}
              >
                RT
              </div>
            </div>
          </div>
        </div>
      </div>
      
      {/* D-pad buttons */}
      <div className="mt-8 flex justify-center">
        <div className="relative h-32 w-32">
          <div 
            id="button-DPadUp" 
            className={cn(
              "controller-button absolute w-10 h-10 top-0 left-1/2 transform -translate-x-1/2",
              buttons.DPadUp && "controller-button-active"
            )}
          >
            ↑
          </div>
          <div 
            id="button-DPadLeft" 
            className={cn(
              "controller-button absolute w-10 h-10 left-0 top-1/2 transform -translate-y-1/2",
              buttons.DPadLeft && "controller-button-active"
            )}
          >
            ←
          </div>
          <div 
            id="button-DPadRight" 
            className={cn(
              "controller-button absolute w-10 h-10 right-0 top-1/2 transform -translate-y-1/2",
              buttons.DPadRight && "controller-button-active"
            )}
          >
            →
          </div>
          <div 
            id="button-DPadDown" 
            className={cn(
              "controller-button absolute w-10 h-10 bottom-0 left-1/2 transform -translate-x-1/2",
              buttons.DPadDown && "controller-button-active"
            )}
          >
            ↓
          </div>
        </div>
      </div>
    </div>
  );
}
