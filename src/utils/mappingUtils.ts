
import { ButtonMapping } from "@/context/ControllerContext";

// Utility to get readable key names
export const getReadableKeyName = (keyCode: string): string => {
  const keyMap: Record<string, string> = {
    'KeyA': 'A',
    'KeyB': 'B',
    'KeyC': 'C',
    'KeyD': 'D',
    'KeyE': 'E',
    'KeyF': 'F',
    'KeyG': 'G',
    'KeyH': 'H',
    'KeyI': 'I',
    'KeyJ': 'J',
    'KeyK': 'K',
    'KeyL': 'L',
    'KeyM': 'M',
    'KeyN': 'N',
    'KeyO': 'O',
    'KeyP': 'P',
    'KeyQ': 'Q',
    'KeyR': 'R',
    'KeyS': 'S',
    'KeyT': 'T',
    'KeyU': 'U',
    'KeyV': 'V',
    'KeyW': 'W',
    'KeyX': 'X',
    'KeyY': 'Y',
    'KeyZ': 'Z',
    'Digit0': '0',
    'Digit1': '1',
    'Digit2': '2',
    'Digit3': '3',
    'Digit4': '4',
    'Digit5': '5',
    'Digit6': '6',
    'Digit7': '7',
    'Digit8': '8',
    'Digit9': '9',
    'Space': 'Space',
    'ShiftLeft': 'Left Shift',
    'ShiftRight': 'Right Shift',
    'ControlLeft': 'Left Ctrl',
    'ControlRight': 'Right Ctrl',
    'AltLeft': 'Left Alt',
    'AltRight': 'Right Alt',
    'Enter': 'Enter',
    'Escape': 'Esc',
    'Backspace': 'Backspace',
    'Tab': 'Tab',
    'CapsLock': 'Caps Lock',
    'F1': 'F1',
    'F2': 'F2',
    'F3': 'F3',
    'F4': 'F4',
    'F5': 'F5',
    'F6': 'F6',
    'F7': 'F7',
    'F8': 'F8',
    'F9': 'F9',
    'F10': 'F10',
    'F11': 'F11',
    'F12': 'F12',
    'ArrowUp': '↑',
    'ArrowDown': '↓',
    'ArrowLeft': '←',
    'ArrowRight': '→',
  };

  return keyMap[keyCode] || keyCode;
};

// Listen for a key press and return the key code
export const listenForKeyPress = (): Promise<string> => {
  return new Promise((resolve) => {
    const handleKeyDown = (e: KeyboardEvent) => {
      e.preventDefault();
      window.removeEventListener('keydown', handleKeyDown);
      resolve(e.code);
    };

    window.addEventListener('keydown', handleKeyDown);
  });
};

// Apply deadzone to analog stick values
export const applyDeadzone = (x: number, y: number, deadzone: number): { x: number, y: number } => {
  const magnitude = Math.sqrt(x * x + y * y);
  
  if (magnitude < deadzone) {
    return { x: 0, y: 0 };
  }

  // Adjust for deadzone
  const normalizedMagnitude = (magnitude - deadzone) / (1 - deadzone);
  const scale = normalizedMagnitude / magnitude;
  
  return {
    x: x * scale,
    y: y * scale
  };
};

// Get color for button based on active state
export const getButtonColor = (button: string, buttonsState: Record<string, boolean>): string => {
  return buttonsState[button] ? 'bg-primary/30 shadow-inner' : 'bg-secondary hover:bg-primary/10';
};

// Get a list of all available controller buttons
export const availableButtons = [
  'A', 'B', 'X', 'Y', 
  'LB', 'RB', 'LT', 'RT', 
  'Back', 'Start', 
  'LS', 'RS',
  'DPadUp', 'DPadDown', 'DPadLeft', 'DPadRight'
];

// Check if a button mapping already exists
export const isButtonAlreadyMapped = (button: string, mappings: ButtonMapping[]): boolean => {
  return mappings.some(mapping => mapping.button === button);
};
