
import { useController } from '@/context/ControllerContext';
import { cn } from '@/lib/utils';

export function StatusBar() {
  const { isEmulationActive } = useController();
  
  return (
    <div className="fixed bottom-0 left-0 right-0 py-2 px-4 bg-white/60 backdrop-blur-lg border-t border-white/20 shadow-sm z-40">
      <div className="container mx-auto flex items-center justify-between">
        <div className="flex items-center gap-4">
          <div className="flex items-center gap-2">
            <div 
              className={cn(
                "h-2 w-2 rounded-full",
                isEmulationActive ? "bg-primary animate-pulse-subtle" : "bg-muted"
              )}
            ></div>
            <span className="text-sm">
              {isEmulationActive ? "Emulation Active" : "Emulation Inactive"}
            </span>
          </div>
          
          <div className="text-xs text-muted-foreground">
            Press F8 to toggle emulation
          </div>
        </div>
        
        <div className="text-xs text-muted-foreground">
          Virtual Controller Emulator • v0.1.0
        </div>
      </div>
    </div>
  );
}
