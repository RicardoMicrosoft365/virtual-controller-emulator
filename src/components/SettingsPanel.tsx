
import { useController } from '@/context/ControllerContext';

export function SettingsPanel() {
  const { settings, updateSettings, analogMappings, updateAnalogMapping } = useController();
  
  const handleSensitivityChange = (value: number) => {
    updateSettings({ mouseSensitivity: value });
    
    // Also update analog mapping
    const rightStickIndex = analogMappings.findIndex(m => m.input === 'mouse' && m.axis === 'right');
    if (rightStickIndex !== -1) {
      const updatedMapping = { ...analogMappings[rightStickIndex], sensitivity: value };
      updateAnalogMapping(rightStickIndex, updatedMapping);
    }
  };
  
  const handleDeadzoneChange = (value: number) => {
    updateSettings({ deadzone: value });
    
    // Also update analog mapping
    const rightStickIndex = analogMappings.findIndex(m => m.input === 'mouse' && m.axis === 'right');
    if (rightStickIndex !== -1) {
      const updatedMapping = { ...analogMappings[rightStickIndex], deadzone: value };
      updateAnalogMapping(rightStickIndex, updatedMapping);
    }
  };
  
  const handleInvertXChange = () => {
    const rightStickIndex = analogMappings.findIndex(m => m.input === 'mouse' && m.axis === 'right');
    if (rightStickIndex !== -1) {
      const invertX = !analogMappings[rightStickIndex].invertX;
      const updatedMapping = { ...analogMappings[rightStickIndex], invertX };
      updateAnalogMapping(rightStickIndex, updatedMapping);
    }
  };
  
  const handleInvertYChange = () => {
    const rightStickIndex = analogMappings.findIndex(m => m.input === 'mouse' && m.axis === 'right');
    if (rightStickIndex !== -1) {
      const invertY = !analogMappings[rightStickIndex].invertY;
      const updatedMapping = { ...analogMappings[rightStickIndex], invertY };
      updateAnalogMapping(rightStickIndex, updatedMapping);
    }
  };
  
  return (
    <div className="w-full max-w-2xl mx-auto">
      <div className="glass-panel p-6">
        <h2 className="text-lg font-medium mb-6">Analog Settings</h2>
        
        <div className="space-y-6">
          <div>
            <div className="flex items-center justify-between mb-2">
              <label className="text-sm font-medium">Mouse Sensitivity</label>
              <span className="text-xs px-2 py-1 rounded-full bg-secondary">
                {settings.mouseSensitivity}%
              </span>
            </div>
            <input
              type="range"
              min="1"
              max="100"
              value={settings.mouseSensitivity}
              onChange={(e) => handleSensitivityChange(parseInt(e.target.value))}
              className="w-full h-2 bg-secondary rounded-lg appearance-none cursor-pointer"
            />
            <div className="flex justify-between text-xs text-muted-foreground mt-1">
              <span>Low</span>
              <span>High</span>
            </div>
          </div>
          
          <div>
            <div className="flex items-center justify-between mb-2">
              <label className="text-sm font-medium">Deadzone</label>
              <span className="text-xs px-2 py-1 rounded-full bg-secondary">
                {settings.deadzone}%
              </span>
            </div>
            <input
              type="range"
              min="0"
              max="50"
              value={settings.deadzone}
              onChange={(e) => handleDeadzoneChange(parseInt(e.target.value))}
              className="w-full h-2 bg-secondary rounded-lg appearance-none cursor-pointer"
            />
            <div className="flex justify-between text-xs text-muted-foreground mt-1">
              <span>None</span>
              <span>High</span>
            </div>
          </div>
          
          <div className="flex items-center gap-8">
            <div className="flex items-center">
              <input
                type="checkbox"
                id="invertX"
                checked={analogMappings.find(m => m.input === 'mouse' && m.axis === 'right')?.invertX || false}
                onChange={handleInvertXChange}
                className="mr-2 h-4 w-4"
              />
              <label htmlFor="invertX" className="text-sm">Invert X-Axis</label>
            </div>
            
            <div className="flex items-center">
              <input
                type="checkbox"
                id="invertY"
                checked={analogMappings.find(m => m.input === 'mouse' && m.axis === 'right')?.invertY || false}
                onChange={handleInvertYChange}
                className="mr-2 h-4 w-4"
              />
              <label htmlFor="invertY" className="text-sm">Invert Y-Axis</label>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
