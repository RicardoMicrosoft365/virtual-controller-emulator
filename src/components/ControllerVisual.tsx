
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
      "xbox-controller max-w-2xl mx-auto transition-opacity duration-300 relative",
      isEmulationActive ? "opacity-100" : "opacity-70"
    )}>
      {/* Xbox logo */}
      <div className="absolute top-14 left-1/2 transform -translate-x-1/2 w-10 h-10 rounded-full bg-black flex items-center justify-center z-10">
        <svg width="18" height="18" viewBox="0 0 24 24" fill="currentColor">
          <path d="M12 0c-0.564 0-1.125 0.123-1.687 0.37-1.664 0.741-3.133 1.658-4.415 2.749-0.249 0.213-0.349 0.483-0.349 0.794 0 0.483 0.152 0.977 0.455 1.48 0.455 0.741 1.077 1.658 1.867 2.749 0.531 0.708 1.077 1.362 1.628 1.934-1.026-1.24-2.667-3.174-3.481-4.204-0.482-0.61-1.542-0.445-2.276 0.333-0.741 0.784-1.644 2.236-2.185 3.358-0.544 1.127-0.878 2.139-0.878 2.26 0 0.058 0.020 0.116 0.053 0.164 0.761 1.534 1.867 2.941 3.253 4.186 1.381 1.24 3.006 2.202 4.827 2.839 0.643 0.164 1.291 0.247 1.934 0.247 0 0 0 0 0 0 0.643 0 1.291-0.082 1.934-0.247 1.821-0.638 3.446-1.599 4.827-2.839 1.386-1.245 2.493-2.654 3.253-4.186 0.033-0.048 0.053-0.105 0.053-0.164 0-0.121-0.334-1.134-0.878-2.26-0.541-1.122-1.443-2.571-2.185-3.358-0.734-0.778-1.8-0.943-2.276-0.333-0.814 1.030-2.455 2.964-3.481 4.204 0.551-0.571 1.097-1.226 1.628-1.934 0.789-1.091 1.412-2.007 1.867-2.749 0.304-0.503 0.455-0.997 0.455-1.48 0-0.317-0.101-0.581-0.349-0.794-1.281-1.091-2.751-2.007-4.415-2.749-0.561-0.247-1.122-0.37-1.687-0.37zM18 10c0 3.313-2.687 6-6 6s-6-2.687-6-6c0-3.313 2.687-6 6-6s6 2.687 6 6z"></path>
        </svg>
      </div>
      
      <div className="grid grid-cols-12 gap-6 items-center pt-10">
        {/* Left Analog Stick */}
        <div className="col-span-4 flex justify-center">
          <div className="text-center">
            <div id="left-stick" className="analog-stick mx-auto mt-2">
              <div 
                ref={leftStickRef}
                id="left-stick-knob" 
                className="analog-knob left-1/2 top-1/2"
              ></div>
            </div>
          </div>
        </div>
        
        {/* D-pad */}
        <div className="col-span-4 flex justify-center">
          <div className="relative h-28 w-28">
            <div 
              id="button-DPadUp" 
              className={cn(
                "absolute w-10 h-10 top-0 left-1/2 transform -translate-x-1/2 bg-black rounded-tl-lg rounded-tr-lg border border-gray-700 flex items-center justify-center",
                buttons.DPadUp && "bg-zinc-700"
              )}
            >
              ↑
            </div>
            <div 
              id="button-DPadLeft" 
              className={cn(
                "absolute w-10 h-10 left-0 top-1/2 transform -translate-y-1/2 bg-black rounded-tl-lg rounded-bl-lg border border-gray-700 flex items-center justify-center",
                buttons.DPadLeft && "bg-zinc-700"
              )}
            >
              ←
            </div>
            <div 
              id="button-DPadRight" 
              className={cn(
                "absolute w-10 h-10 right-0 top-1/2 transform -translate-y-1/2 bg-black rounded-tr-lg rounded-br-lg border border-gray-700 flex items-center justify-center",
                buttons.DPadRight && "bg-zinc-700"
              )}
            >
              →
            </div>
            <div 
              id="button-DPadDown" 
              className={cn(
                "absolute w-10 h-10 bottom-0 left-1/2 transform -translate-x-1/2 bg-black rounded-bl-lg rounded-br-lg border border-gray-700 flex items-center justify-center",
                buttons.DPadDown && "bg-zinc-700"
              )}
            >
              ↓
            </div>
            <div 
              className="absolute w-10 h-10 left-1/2 top-1/2 transform -translate-x-1/2 -translate-y-1/2 bg-black border border-gray-700"
            ></div>
          </div>
        </div>
        
        {/* Right Analog Stick */}
        <div className="col-span-4 flex justify-center">
          <div className="text-center">
            <div id="right-stick" className="analog-stick mx-auto mt-2">
              <div 
                ref={rightStickRef}
                id="right-stick-knob" 
                className="analog-knob left-1/2 top-1/2"
              ></div>
            </div>
          </div>
        </div>
      </div>
      
      {/* Menu buttons and Face buttons */}
      <div className="grid grid-cols-12 gap-4 mt-6">
        {/* Menu buttons */}
        <div className="col-span-6 flex justify-center gap-8">
          <button 
            id="button-Back" 
            className={cn(
              "w-8 h-8 bg-black border border-gray-700 rounded-full flex items-center justify-center",
              buttons.Back && "bg-zinc-700"
            )}
          >
            <svg width="14" height="14" viewBox="0 0 24 24" fill="currentColor">
              <rect x="4" y="8" width="16" height="2" rx="1" />
              <rect x="4" y="14" width="16" height="2" rx="1" />
            </svg>
          </button>
          
          <button 
            id="button-Start" 
            className={cn(
              "w-8 h-8 bg-black border border-gray-700 rounded-full flex items-center justify-center",
              buttons.Start && "bg-zinc-700"
            )}
          >
            <svg width="14" height="14" viewBox="0 0 24 24" fill="currentColor">
              <rect x="7" y="7" width="10" height="10" rx="1" />
            </svg>
          </button>
        </div>
        
        {/* Face buttons */}
        <div className="col-span-6">
          <div className="relative h-36 w-36 mx-auto">
            <div 
              id="button-Y" 
              className={cn(
                "xbox-button-y absolute top-0 left-1/2 transform -translate-x-1/2",
                buttons.Y && "bg-zinc-700"
              )}
            >
              Y
            </div>
            <div 
              id="button-X" 
              className={cn(
                "xbox-button-x absolute left-0 top-1/2 transform -translate-y-1/2",
                buttons.X && "bg-zinc-700"
              )}
            >
              X
            </div>
            <div 
              id="button-B" 
              className={cn(
                "xbox-button-b absolute right-0 top-1/2 transform -translate-y-1/2",
                buttons.B && "bg-zinc-700"
              )}
            >
              B
            </div>
            <div 
              id="button-A" 
              className={cn(
                "xbox-button-a absolute bottom-0 left-1/2 transform -translate-x-1/2",
                buttons.A && "bg-zinc-700"
              )}
            >
              A
            </div>
          </div>
        </div>
      </div>
      
      {/* Shoulder buttons */}
      <div className="grid grid-cols-12 gap-6 mt-4">
        <div className="col-span-6 flex justify-start pl-8">
          <div>
            <div 
              id="button-LB" 
              className={cn(
                "bg-black border border-gray-700 rounded-lg w-20 h-8 mb-2 flex items-center justify-center",
                buttons.LB && "bg-zinc-700"
              )}
            >
              LB
            </div>
            <div 
              id="button-LT" 
              className={cn(
                "bg-black border border-gray-700 rounded-lg w-20 h-8 flex items-center justify-center",
                buttons.LT && "bg-zinc-700"
              )}
            >
              LT
            </div>
          </div>
        </div>
        
        <div className="col-span-6 flex justify-end pr-8">
          <div>
            <div 
              id="button-RB" 
              className={cn(
                "bg-black border border-gray-700 rounded-lg w-20 h-8 mb-2 flex items-center justify-center",
                buttons.RB && "bg-zinc-700"
              )}
            >
              RB
            </div>
            <div 
              id="button-RT" 
              className={cn(
                "bg-black border border-gray-700 rounded-lg w-20 h-8 flex items-center justify-center",
                buttons.RT && "bg-zinc-700"
              )}
            >
              RT
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
