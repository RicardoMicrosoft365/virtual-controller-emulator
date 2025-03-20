
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
      "relative max-w-4xl mx-auto transition-opacity duration-300",
      isEmulationActive ? "opacity-100" : "opacity-70"
    )}>
      {/* Xbox controller background image */}
      <div className="relative">
        <img 
          src="/xbox-controller.png" 
          alt="Xbox Controller" 
          className="w-full h-auto"
        />
        
        {/* Virtual controller elements positioned on top of the image */}
        <div className="absolute inset-0">
          {/* Left Analog Stick */}
          <div 
            className="absolute left-[17%] top-[28%] w-[16%] h-[16%]"
          >
            <div id="left-stick" className="analog-stick w-full h-full">
              <div 
                ref={leftStickRef}
                id="left-stick-knob" 
                className="analog-knob w-[40%] h-[40%] left-1/2 top-1/2"
              ></div>
            </div>
          </div>
          
          {/* Right Analog Stick */}
          <div 
            className="absolute right-[28%] top-[40%] w-[16%] h-[16%]"
          >
            <div id="right-stick" className="analog-stick w-full h-full">
              <div 
                ref={rightStickRef}
                id="right-stick-knob" 
                className="analog-knob w-[40%] h-[40%] left-1/2 top-1/2"
              ></div>
            </div>
          </div>
          
          {/* D-pad */}
          <div className="absolute left-[29%] top-[40%] w-[14%] h-[14%]">
            <div 
              id="button-DPadUp" 
              className={cn(
                "absolute w-[33%] h-[33%] top-0 left-1/2 transform -translate-x-1/2 rounded-tl-lg rounded-tr-lg flex items-center justify-center opacity-60",
                buttons.DPadUp ? "bg-primary" : "bg-transparent"
              )}
            >
              <span className="text-xs">↑</span>
            </div>
            <div 
              id="button-DPadLeft" 
              className={cn(
                "absolute w-[33%] h-[33%] left-0 top-1/2 transform -translate-y-1/2 rounded-tl-lg rounded-bl-lg flex items-center justify-center opacity-60",
                buttons.DPadLeft ? "bg-primary" : "bg-transparent"
              )}
            >
              <span className="text-xs">←</span>
            </div>
            <div 
              id="button-DPadRight" 
              className={cn(
                "absolute w-[33%] h-[33%] right-0 top-1/2 transform -translate-y-1/2 rounded-tr-lg rounded-br-lg flex items-center justify-center opacity-60",
                buttons.DPadRight ? "bg-primary" : "bg-transparent"
              )}
            >
              <span className="text-xs">→</span>
            </div>
            <div 
              id="button-DPadDown" 
              className={cn(
                "absolute w-[33%] h-[33%] bottom-0 left-1/2 transform -translate-x-1/2 rounded-bl-lg rounded-br-lg flex items-center justify-center opacity-60",
                buttons.DPadDown ? "bg-primary" : "bg-transparent"
              )}
            >
              <span className="text-xs">↓</span>
            </div>
          </div>
          
          {/* Xbox logo button */}
          <div className="absolute top-[28%] left-[47%] transform -translate-x-1/2 w-[6%] h-[6%]">
            <div 
              id="button-Guide" 
              className={cn(
                "rounded-full w-full h-full flex items-center justify-center",
                buttons.Guide ? "bg-green-500/60" : "bg-transparent"
              )}
            >
            </div>
          </div>
          
          {/* Menu buttons (Back/View & Start/Menu) */}
          <div className="absolute top-[28%] left-[38%] w-[5%] h-[5%]">
            <button 
              id="button-Back" 
              className={cn(
                "w-full h-full rounded-full flex items-center justify-center opacity-60",
                buttons.Back ? "bg-primary" : "bg-transparent"
              )}
            >
            </button>
          </div>
          
          <div className="absolute top-[28%] right-[38%] w-[5%] h-[5%]">
            <button 
              id="button-Start" 
              className={cn(
                "w-full h-full rounded-full flex items-center justify-center opacity-60",
                buttons.Start ? "bg-primary" : "bg-transparent"
              )}
            >
            </button>
          </div>
          
          {/* Face buttons */}
          <div className="absolute right-[17%] top-[28%] w-[20%] h-[20%]">
            {/* A Button (bottom) */}
            <div 
              id="button-A" 
              className={cn(
                "absolute bottom-0 left-1/2 transform -translate-x-1/2 w-[40%] h-[40%] rounded-full flex items-center justify-center opacity-60",
                buttons.A ? "bg-green-500" : "bg-transparent"
              )}
            >
              <span className="text-white font-bold">A</span>
            </div>
            
            {/* B Button (right) */}
            <div 
              id="button-B" 
              className={cn(
                "absolute right-0 top-1/2 transform -translate-y-1/2 w-[40%] h-[40%] rounded-full flex items-center justify-center opacity-60",
                buttons.B ? "bg-red-500" : "bg-transparent"
              )}
            >
              <span className="text-white font-bold">B</span>
            </div>
            
            {/* X Button (left) */}
            <div 
              id="button-X" 
              className={cn(
                "absolute left-0 top-1/2 transform -translate-y-1/2 w-[40%] h-[40%] rounded-full flex items-center justify-center opacity-60",
                buttons.X ? "bg-blue-500" : "bg-transparent"
              )}
            >
              <span className="text-white font-bold">X</span>
            </div>
            
            {/* Y Button (top) */}
            <div 
              id="button-Y" 
              className={cn(
                "absolute top-0 left-1/2 transform -translate-x-1/2 w-[40%] h-[40%] rounded-full flex items-center justify-center opacity-60",
                buttons.Y ? "bg-yellow-500" : "bg-transparent"
              )}
            >
              <span className="text-white font-bold">Y</span>
            </div>
          </div>
          
          {/* Shoulder buttons */}
          <div className="absolute left-[15%] top-[10%] w-[20%] h-[10%]">
            <div 
              id="button-LB" 
              className={cn(
                "w-full h-full opacity-60",
                buttons.LB ? "bg-primary/50" : "bg-transparent"
              )}
            >
            </div>
          </div>
          
          <div className="absolute right-[15%] top-[10%] w-[20%] h-[10%]">
            <div 
              id="button-RB" 
              className={cn(
                "w-full h-full opacity-60",
                buttons.RB ? "bg-primary/50" : "bg-transparent"
              )}
            >
            </div>
          </div>
          
          {/* Trigger buttons (positioned at the top) */}
          <div className="absolute left-[15%] top-[2%] w-[20%] h-[8%]">
            <div 
              id="button-LT" 
              className={cn(
                "w-full h-full opacity-60 rounded-lg",
                buttons.LT ? "bg-primary/50" : "bg-transparent"
              )}
            >
            </div>
          </div>
          
          <div className="absolute right-[15%] top-[2%] w-[20%] h-[8%]">
            <div 
              id="button-RT" 
              className={cn(
                "w-full h-full opacity-60 rounded-lg",
                buttons.RT ? "bg-primary/50" : "bg-transparent"
              )}
            >
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
