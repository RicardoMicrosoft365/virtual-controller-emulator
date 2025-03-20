
import React, { createContext, useContext, useState, useEffect } from 'react';

export type ButtonMapping = {
  key: string;
  button: string;
  active: boolean;
};

export type AnalogMapping = {
  input: 'mouse' | 'keys';
  axis: 'left' | 'right';
  sensitivity: number;
  deadzone: number;
  invertX: boolean;
  invertY: boolean;
};

export type ControllerState = {
  isActive: boolean;
  leftStick: { x: number; y: number };
  rightStick: { x: number; y: number };
  buttons: Record<string, boolean>;
};

interface ControllerContextType {
  isEmulationActive: boolean;
  toggleEmulation: () => void;
  buttonMappings: ButtonMapping[];
  analogMappings: AnalogMapping[];
  controllerState: ControllerState;
  updateButtonMapping: (index: number, mapping: ButtonMapping) => void;
  addButtonMapping: (mapping: ButtonMapping) => void;
  removeButtonMapping: (index: number) => void;
  updateAnalogMapping: (index: number, mapping: AnalogMapping) => void;
  updateControllerState: (state: Partial<ControllerState>) => void;
  settings: {
    mouseSensitivity: number;
    deadzone: number;
  };
  updateSettings: (settings: Partial<typeof defaultSettings>) => void;
}

const defaultSettings = {
  mouseSensitivity: 50,
  deadzone: 10,
};

const defaultButtonMappings: ButtonMapping[] = [
  { key: 'Space', button: 'A', active: false },
  { key: 'ShiftLeft', button: 'B', active: false },
  { key: 'KeyE', button: 'X', active: false },
  { key: 'KeyQ', button: 'Y', active: false },
  { key: 'KeyR', button: 'RB', active: false },
  { key: 'KeyF', button: 'LB', active: false },
];

const defaultAnalogMappings: AnalogMapping[] = [
  {
    input: 'mouse',
    axis: 'right',
    sensitivity: 50,
    deadzone: 10,
    invertX: false,
    invertY: false,
  },
  {
    input: 'keys',
    axis: 'left',
    sensitivity: 100,
    deadzone: 5,
    invertX: false,
    invertY: false,
  },
];

const defaultControllerState: ControllerState = {
  isActive: false,
  leftStick: { x: 0, y: 0 },
  rightStick: { x: 0, y: 0 },
  buttons: {
    A: false,
    B: false,
    X: false,
    Y: false,
    LB: false,
    RB: false,
    LT: false,
    RT: false,
    Back: false,
    Start: false,
    LS: false,
    RS: false,
    DPadUp: false,
    DPadDown: false,
    DPadLeft: false,
    DPadRight: false,
  },
};

const ControllerContext = createContext<ControllerContextType | undefined>(undefined);

export const ControllerProvider: React.FC<{ children: React.ReactNode }> = ({ children }) => {
  const [isEmulationActive, setIsEmulationActive] = useState(false);
  const [buttonMappings, setButtonMappings] = useState<ButtonMapping[]>(defaultButtonMappings);
  const [analogMappings, setAnalogMappings] = useState<AnalogMapping[]>(defaultAnalogMappings);
  const [controllerState, setControllerState] = useState<ControllerState>(defaultControllerState);
  const [settings, setSettings] = useState(defaultSettings);

  const toggleEmulation = () => {
    setIsEmulationActive((prev) => !prev);
  };

  const updateButtonMapping = (index: number, mapping: ButtonMapping) => {
    setButtonMappings((prev) => {
      const newMappings = [...prev];
      newMappings[index] = mapping;
      return newMappings;
    });
  };

  const addButtonMapping = (mapping: ButtonMapping) => {
    setButtonMappings((prev) => [...prev, mapping]);
  };

  const removeButtonMapping = (index: number) => {
    setButtonMappings((prev) => prev.filter((_, i) => i !== index));
  };

  const updateAnalogMapping = (index: number, mapping: AnalogMapping) => {
    setAnalogMappings((prev) => {
      const newMappings = [...prev];
      newMappings[index] = mapping;
      return newMappings;
    });
  };

  const updateControllerState = (state: Partial<ControllerState>) => {
    setControllerState((prev) => ({ ...prev, ...state }));
  };

  const updateSettings = (newSettings: Partial<typeof defaultSettings>) => {
    setSettings((prev) => ({ ...prev, ...newSettings }));
  };

  // Save/load state from localStorage
  useEffect(() => {
    try {
      const savedData = localStorage.getItem('controllerConfig');
      if (savedData) {
        const { buttonMappings: saved, analogMappings: savedAnalog, settings: savedSettings } = JSON.parse(savedData);
        if (saved) setButtonMappings(saved);
        if (savedAnalog) setAnalogMappings(savedAnalog);
        if (savedSettings) setSettings(savedSettings);
      }
    } catch (error) {
      console.error('Error loading saved configuration', error);
    }
  }, []);

  useEffect(() => {
    try {
      localStorage.setItem('controllerConfig', JSON.stringify({
        buttonMappings,
        analogMappings,
        settings,
      }));
    } catch (error) {
      console.error('Error saving configuration', error);
    }
  }, [buttonMappings, analogMappings, settings]);

  return (
    <ControllerContext.Provider 
      value={{
        isEmulationActive,
        toggleEmulation,
        buttonMappings,
        analogMappings,
        controllerState,
        updateButtonMapping,
        addButtonMapping,
        removeButtonMapping,
        updateAnalogMapping,
        updateControllerState,
        settings,
        updateSettings,
      }}
    >
      {children}
    </ControllerContext.Provider>
  );
};

export const useController = () => {
  const context = useContext(ControllerContext);
  if (context === undefined) {
    throw new Error('useController must be used within a ControllerProvider');
  }
  return context;
};
