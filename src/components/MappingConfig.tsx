
import { useState } from 'react';
import { useController } from '@/context/ControllerContext';
import { listenForKeyPress, getReadableKeyName, availableButtons, isButtonAlreadyMapped } from '@/utils/mappingUtils';
import { cn } from '@/lib/utils';

export function MappingConfig() {
  const { buttonMappings, updateButtonMapping, addButtonMapping, removeButtonMapping } = useController();
  const [isListening, setIsListening] = useState(false);
  const [currentIndex, setCurrentIndex] = useState<number | null>(null);
  const [currentButtonSelection, setCurrentButtonSelection] = useState('');
  const [showAddModal, setShowAddModal] = useState(false);
  const [newMapping, setNewMapping] = useState({ key: '', button: '' });

  const handleRemoveMapping = (index: number) => {
    removeButtonMapping(index);
  };

  const handleEditKey = async (index: number) => {
    setIsListening(true);
    setCurrentIndex(index);
    
    try {
      const keyCode = await listenForKeyPress();
      const updatedMapping = { ...buttonMappings[index], key: keyCode };
      updateButtonMapping(index, updatedMapping);
    } catch (error) {
      console.error('Error capturing key press', error);
    } finally {
      setIsListening(false);
      setCurrentIndex(null);
    }
  };

  const handleEditButton = (index: number) => {
    setCurrentIndex(index);
    setCurrentButtonSelection(buttonMappings[index].button);
  };

  const selectButton = (button: string) => {
    if (currentIndex === null) {
      // For new mapping
      setNewMapping({ ...newMapping, button });
    } else {
      // For existing mapping
      const updatedMapping = { ...buttonMappings[currentIndex], button };
      updateButtonMapping(currentIndex, updatedMapping);
      setCurrentIndex(null);
    }
    setCurrentButtonSelection('');
  };

  const handleStartAddMapping = () => {
    setShowAddModal(true);
    setNewMapping({ key: '', button: '' });
  };

  const handleCaptureNewKey = async () => {
    setIsListening(true);
    
    try {
      const keyCode = await listenForKeyPress();
      setNewMapping({ ...newMapping, key: keyCode });
    } catch (error) {
      console.error('Error capturing key press', error);
    } finally {
      setIsListening(false);
    }
  };

  const handleSaveNewMapping = () => {
    if (newMapping.key && newMapping.button) {
      addButtonMapping({ ...newMapping, active: false });
      setShowAddModal(false);
    }
  };

  return (
    <div className="w-full max-w-2xl mx-auto">
      <div className="glass-panel p-6">
        <div className="flex items-center justify-between mb-6">
          <h2 className="text-lg font-medium">Button Mappings</h2>
          <button
            onClick={handleStartAddMapping}
            className="glass-button focus-effect text-sm"
          >
            Add Mapping
          </button>
        </div>

        <div className="space-y-3">
          {buttonMappings.map((mapping, index) => (
            <div 
              key={index} 
              className="mapping-item hover:bg-white/80 focus-effect"
            >
              <div className="flex-1 flex items-center justify-between">
                <div className="flex items-center gap-4">
                  <div className="text-sm font-medium">{getReadableKeyName(mapping.key)}</div>
                  <div className="text-muted-foreground">→</div>
                  <div className="text-sm font-medium">{mapping.button}</div>
                </div>
                
                <div className="flex items-center gap-2">
                  <button
                    onClick={() => handleEditKey(index)}
                    className="px-3 py-1 text-xs rounded-md bg-secondary hover:bg-secondary/80 transition-colors focus-effect"
                    disabled={isListening}
                  >
                    {currentIndex === index && isListening ? 'Press a key...' : 'Edit Key'}
                  </button>
                  
                  <button
                    onClick={() => handleEditButton(index)}
                    className="px-3 py-1 text-xs rounded-md bg-secondary hover:bg-secondary/80 transition-colors focus-effect"
                  >
                    Edit Button
                  </button>
                  
                  <button
                    onClick={() => handleRemoveMapping(index)}
                    className="p-1 text-xs rounded-md text-destructive hover:bg-destructive/10 transition-colors focus-effect"
                  >
                    <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
                      <path d="M3 6h18"></path>
                      <path d="M19 6v14c0 1-1 2-2 2H7c-1 0-2-1-2-2V6"></path>
                      <path d="M8 6V4c0-1 1-2 2-2h4c1 0 2 1 2 2v2"></path>
                    </svg>
                  </button>
                </div>
              </div>
            </div>
          ))}

          {buttonMappings.length === 0 && (
            <div className="text-center py-8 text-muted-foreground">
              No button mappings configured. Click "Add Mapping" to create one.
            </div>
          )}
        </div>
      </div>

      {/* Button Selection Dropdown */}
      {currentButtonSelection || currentIndex !== null ? (
        <div className="fixed inset-0 bg-black/20 backdrop-blur-sm z-50 flex items-center justify-center">
          <div className="glass-panel p-6 w-96 animate-fade-in">
            <h3 className="text-lg font-medium mb-4">Select Controller Button</h3>
            <div className="grid grid-cols-4 gap-2 mb-4">
              {availableButtons.map((button) => (
                <button
                  key={button}
                  onClick={() => selectButton(button)}
                  disabled={isButtonAlreadyMapped(button, buttonMappings) && 
                            (currentIndex === null || buttonMappings[currentIndex].button !== button)}
                  className={cn(
                    "p-2 rounded-md text-sm transition-all focus-effect",
                    isButtonAlreadyMapped(button, buttonMappings) && 
                    (currentIndex === null || buttonMappings[currentIndex].button !== button)
                      ? "bg-muted/50 text-muted-foreground cursor-not-allowed"
                      : "bg-secondary hover:bg-secondary/80"
                  )}
                >
                  {button}
                </button>
              ))}
            </div>
            <button
              onClick={() => {
                setCurrentIndex(null);
                setCurrentButtonSelection('');
              }}
              className="w-full glass-button focus-effect"
            >
              Cancel
            </button>
          </div>
        </div>
      ) : null}

      {/* Add New Mapping Modal */}
      {showAddModal && (
        <div className="fixed inset-0 bg-black/20 backdrop-blur-sm z-50 flex items-center justify-center">
          <div className="glass-panel p-6 w-96 animate-fade-in">
            <h3 className="text-lg font-medium mb-4">Add New Mapping</h3>
            
            <div className="space-y-4 mb-6">
              <div>
                <label className="block text-sm font-medium mb-1">Keyboard Key</label>
                <button
                  onClick={handleCaptureNewKey}
                  disabled={isListening}
                  className={cn(
                    "w-full p-2 rounded-md text-sm glass-input focus-effect text-left",
                    isListening && "bg-secondary/50"
                  )}
                >
                  {isListening 
                    ? "Press a key..." 
                    : newMapping.key 
                      ? getReadableKeyName(newMapping.key) 
                      : "Click to capture key"}
                </button>
              </div>
              
              <div>
                <label className="block text-sm font-medium mb-1">Controller Button</label>
                <button
                  onClick={() => setCurrentButtonSelection('new')}
                  className="w-full p-2 rounded-md text-sm glass-input focus-effect text-left"
                >
                  {newMapping.button || "Select a button"}
                </button>
              </div>
            </div>
            
            <div className="flex gap-2">
              <button
                onClick={() => setShowAddModal(false)}
                className="flex-1 p-2 bg-secondary rounded-md hover:bg-secondary/80 transition-colors focus-effect"
              >
                Cancel
              </button>
              <button
                onClick={handleSaveNewMapping}
                disabled={!newMapping.key || !newMapping.button}
                className={cn(
                  "flex-1 p-2 rounded-md transition-colors focus-effect",
                  newMapping.key && newMapping.button
                    ? "bg-primary text-primary-foreground hover:bg-primary/90"
                    : "bg-primary/50 text-primary-foreground cursor-not-allowed"
                )}
              >
                Save
              </button>
            </div>
          </div>
        </div>
      )}
    </div>
  );
}
