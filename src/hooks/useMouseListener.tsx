
import { useEffect, useRef } from 'react';
import { useController } from '@/context/ControllerContext';

export function useMouseListener() {
  const { isEmulationActive, analogMappings, settings, updateControllerState } = useController();
  const mousePositionRef = useRef({ x: 0, y: 0 });
  
  useEffect(() => {
    if (!isEmulationActive) return;

    const rightStickMapping = analogMappings.find(m => m.input === 'mouse' && m.axis === 'right');
    if (!rightStickMapping) return;

    const sensitivity = rightStickMapping.sensitivity / 50; // Convert 0-100 to a multiplier
    const deadzone = rightStickMapping.deadzone / 100; // Convert to percentage

    const handleMouseMove = (e: MouseEvent) => {
      // Calculate mouse delta
      const deltaX = e.movementX * sensitivity;
      const deltaY = e.movementY * sensitivity;
      
      // Store current position
      mousePositionRef.current = {
        x: mousePositionRef.current.x + deltaX,
        y: mousePositionRef.current.y + deltaY
      };
      
      // Apply deadzone and normalization
      let x = mousePositionRef.current.x / 100; // Normalize to -1 to 1
      let y = mousePositionRef.current.y / 100;
      
      // Constrain values between -1 and 1
      x = Math.max(-1, Math.min(1, x));
      y = Math.max(-1, Math.min(1, y));
      
      // Apply deadzone
      const magnitude = Math.sqrt(x * x + y * y);
      if (magnitude < deadzone) {
        x = 0;
        y = 0;
      } else {
        // Adjust for deadzone
        const normalizedMagnitude = (magnitude - deadzone) / (1 - deadzone);
        const scale = normalizedMagnitude / magnitude;
        x *= scale;
        y *= scale;
      }
      
      // Apply inversion if needed
      if (rightStickMapping.invertX) x = -x;
      if (rightStickMapping.invertY) y = -y;
      
      // Update controller state
      updateControllerState({
        rightStick: { x, y }
      });
      
      // Update stick position in the UI
      const stick = document.getElementById('right-stick-knob');
      if (stick) {
        const stickContainer = document.getElementById('right-stick');
        if (stickContainer) {
          const centerX = stickContainer.clientWidth / 2;
          const centerY = stickContainer.clientHeight / 2;
          const maxDistance = stickContainer.clientWidth / 3;
          
          stick.style.left = `${centerX + x * maxDistance}px`;
          stick.style.top = `${centerY + y * maxDistance}px`;
        }
      }
    };

    // Reset position when emulation is toggled
    const resetMousePosition = () => {
      mousePositionRef.current = { x: 0, y: 0 };
      updateControllerState({
        rightStick: { x: 0, y: 0 }
      });
      
      // Reset stick position in the UI
      const stick = document.getElementById('right-stick-knob');
      if (stick) {
        const stickContainer = document.getElementById('right-stick');
        if (stickContainer) {
          const centerX = stickContainer.clientWidth / 2;
          const centerY = stickContainer.clientHeight / 2;
          
          stick.style.left = `${centerX}px`;
          stick.style.top = `${centerY}px`;
        }
      }
    };

    // Register event listeners
    window.addEventListener('mousemove', handleMouseMove);
    
    // Cleanup
    return () => {
      window.removeEventListener('mousemove', handleMouseMove);
      resetMousePosition();
    };
  }, [isEmulationActive, analogMappings, settings, updateControllerState]);
}
