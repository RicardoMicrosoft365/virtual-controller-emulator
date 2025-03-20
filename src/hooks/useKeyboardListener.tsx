
import { useEffect } from 'react';
import { useController } from '@/context/ControllerContext';

export function useKeyboardListener() {
  const { isEmulationActive, buttonMappings, toggleEmulation, updateControllerState } = useController();

  useEffect(() => {
    const handleKeyDown = (e: KeyboardEvent) => {
      // Toggle emulation with F8
      if (e.code === 'F8') {
        toggleEmulation();
        return;
      }

      if (!isEmulationActive) return;

      // Find mapped buttons for this key
      const mapping = buttonMappings.find((m) => m.key === e.code);
      if (mapping) {
        updateControllerState({
          buttons: {
            [mapping.button]: true,
          },
        });

        // Update active state in mapping
        const index = buttonMappings.findIndex((m) => m.key === e.code);
        if (index !== -1) {
          const updatedMapping = { ...buttonMappings[index], active: true };
          // This will just update the UI, not the actual mapping
          const element = document.getElementById(`button-${mapping.button}`);
          if (element) {
            element.classList.add('controller-button-active');
          }
        }
      }
    };

    const handleKeyUp = (e: KeyboardEvent) => {
      if (!isEmulationActive) return;

      // Find mapped buttons for this key
      const mapping = buttonMappings.find((m) => m.key === e.code);
      if (mapping) {
        updateControllerState({
          buttons: {
            [mapping.button]: false,
          },
        });

        // Update active state in mapping
        const index = buttonMappings.findIndex((m) => m.key === e.code);
        if (index !== -1) {
          const updatedMapping = { ...buttonMappings[index], active: false };
          // This will just update the UI, not the actual mapping
          const element = document.getElementById(`button-${mapping.button}`);
          if (element) {
            element.classList.remove('controller-button-active');
          }
        }
      }
    };

    // Register event listeners
    window.addEventListener('keydown', handleKeyDown);
    window.addEventListener('keyup', handleKeyUp);

    // Cleanup
    return () => {
      window.removeEventListener('keydown', handleKeyDown);
      window.removeEventListener('keyup', handleKeyUp);
    };
  }, [isEmulationActive, buttonMappings, toggleEmulation, updateControllerState]);
}
