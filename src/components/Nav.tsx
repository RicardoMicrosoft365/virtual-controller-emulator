
import { useState, useEffect } from 'react';
import { useController } from '@/context/ControllerContext';
import { cn } from '@/lib/utils';

export function Nav() {
  const { isEmulationActive, toggleEmulation } = useController();
  const [scrolled, setScrolled] = useState(false);

  useEffect(() => {
    const handleScroll = () => {
      setScrolled(window.scrollY > 10);
    };

    window.addEventListener('scroll', handleScroll);
    return () => window.removeEventListener('scroll', handleScroll);
  }, []);

  return (
    <header 
      className={cn(
        "fixed top-0 left-0 right-0 z-50 transition-all duration-300 py-4 px-6",
        scrolled ? "bg-white/80 backdrop-blur-md shadow-sm" : "bg-transparent"
      )}
    >
      <div className="container mx-auto flex items-center justify-between">
        <div className="flex items-center gap-2">
          <div className="h-8 w-8 bg-primary rounded-lg animate-pulse-subtle"></div>
          <h1 className="text-xl font-medium">Virtual Controller</h1>
        </div>
        
        <div className="flex items-center gap-4">
          <div 
            className={cn(
              "px-3 py-1 rounded-full text-sm font-medium transition-all duration-300 flex items-center gap-2",
              isEmulationActive 
                ? "bg-primary/10 text-primary" 
                : "bg-secondary/80 text-muted-foreground"
            )}
          >
            <div 
              className={cn(
                "h-2 w-2 rounded-full transition-colors duration-300", 
                isEmulationActive ? "bg-primary animate-pulse-subtle" : "bg-muted-foreground"
              )}
            ></div>
            {isEmulationActive ? "Active" : "Inactive"}
          </div>
          
          <button 
            onClick={toggleEmulation} 
            className="glass-button focus-effect"
          >
            {isEmulationActive ? "Deactivate (F8)" : "Activate (F8)"}
          </button>
        </div>
      </div>
    </header>
  );
}
