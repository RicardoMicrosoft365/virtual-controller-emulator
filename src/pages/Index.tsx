
import { useEffect } from "react";
import { ControllerProvider } from "@/context/ControllerContext";
import { useKeyboardListener } from "@/hooks/useKeyboardListener";
import { useMouseListener } from "@/hooks/useMouseListener";
import { Nav } from "@/components/Nav";
import { ControllerVisual } from "@/components/ControllerVisual";
import { MappingConfig } from "@/components/MappingConfig";
import { SettingsPanel } from "@/components/SettingsPanel";
import { StatusBar } from "@/components/StatusBar";

const EmulatorContent = () => {
  useKeyboardListener();
  useMouseListener();

  useEffect(() => {
    document.title = "Virtual Controller Emulator";
  }, []);

  return (
    <div className="min-h-screen pt-24 pb-16 px-4">
      <Nav />
      
      <div className="container mx-auto space-y-8 animate-fade-in">
        <div>
          <span className="inline-block py-1 px-3 text-xs font-medium rounded-full bg-primary/10 text-primary mb-4">Real-time Visualization</span>
          <h2 className="text-2xl font-medium mb-8">Controller State</h2>
          <ControllerVisual />
        </div>
        
        <div className="grid md:grid-cols-2 gap-8">
          <div>
            <span className="inline-block py-1 px-3 text-xs font-medium rounded-full bg-primary/10 text-primary mb-4">Keyboard to Controller</span>
            <h2 className="text-2xl font-medium mb-6">Button Mappings</h2>
            <MappingConfig />
          </div>
          
          <div>
            <span className="inline-block py-1 px-3 text-xs font-medium rounded-full bg-primary/10 text-primary mb-4">Mouse to Analog</span>
            <h2 className="text-2xl font-medium mb-6">Sensitivity Settings</h2>
            <SettingsPanel />
          </div>
        </div>
      </div>
      
      <StatusBar />
    </div>
  );
};

const Index = () => {
  return (
    <ControllerProvider>
      <EmulatorContent />
    </ControllerProvider>
  );
};

export default Index;
