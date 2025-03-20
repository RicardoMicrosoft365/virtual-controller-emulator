
import { useState, useEffect } from 'react';
import { useController } from '@/context/ControllerContext';
import { cn } from '@/lib/utils';
import { useNavigate } from 'react-router-dom';
import { LogOut } from 'lucide-react';

export function Nav() {
  const { isEmulationActive, toggleEmulation } = useController();
  const [scrolled, setScrolled] = useState(false);
  const navigate = useNavigate();

  useEffect(() => {
    const handleScroll = () => {
      setScrolled(window.scrollY > 10);
    };

    window.addEventListener('scroll', handleScroll);
    return () => window.removeEventListener('scroll', handleScroll);
  }, []);

  const handleLogout = () => {
    localStorage.removeItem('isAuthenticated');
    navigate('/login');
  };

  return (
    <header 
      className={cn(
        "fixed top-0 left-0 right-0 z-50 transition-all duration-300 py-4 px-6",
        scrolled 
          ? "bg-white/80 backdrop-blur-md shadow-sm dark:bg-black/80" 
          : "bg-transparent"
      )}
    >
      <div className="container mx-auto flex items-center justify-between">
        <div className="flex items-center gap-2">
          <div className="h-8 w-8 bg-zinc-900 text-white rounded-full flex items-center justify-center">
            <svg width="18" height="18" viewBox="0 0 24 24" fill="currentColor">
              <path d="M12 0c-0.564 0-1.125 0.123-1.687 0.37-1.664 0.741-3.133 1.658-4.415 2.749-0.249 0.213-0.349 0.483-0.349 0.794 0 0.483 0.152 0.977 0.455 1.48 0.455 0.741 1.077 1.658 1.867 2.749 0.531 0.708 1.077 1.362 1.628 1.934-1.026-1.24-2.667-3.174-3.481-4.204-0.482-0.61-1.542-0.445-2.276 0.333-0.741 0.784-1.644 2.236-2.185 3.358-0.544 1.127-0.878 2.139-0.878 2.26 0 0.058 0.020 0.116 0.053 0.164 0.761 1.534 1.867 2.941 3.253 4.186 1.381 1.24 3.006 2.202 4.827 2.839 0.643 0.164 1.291 0.247 1.934 0.247 0 0 0 0 0 0 0.643 0 1.291-0.082 1.934-0.247 1.821-0.638 3.446-1.599 4.827-2.839 1.386-1.245 2.493-2.654 3.253-4.186 0.033-0.048 0.053-0.105 0.053-0.164 0-0.121-0.334-1.134-0.878-2.26-0.541-1.122-1.443-2.571-2.185-3.358-0.734-0.778-1.8-0.943-2.276-0.333-0.814 1.030-2.455 2.964-3.481 4.204 0.551-0.571 1.097-1.226 1.628-1.934 0.789-1.091 1.412-2.007 1.867-2.749 0.304-0.503 0.455-0.997 0.455-1.48 0-0.317-0.101-0.581-0.349-0.794-1.281-1.091-2.751-2.007-4.415-2.749-0.561-0.247-1.122-0.37-1.687-0.37zM18 10c0 3.313-2.687 6-6 6s-6-2.687-6-6c0-3.313 2.687-6 6-6s6 2.687 6 6z"></path>
            </svg>
          </div>
          <h1 className="text-xl font-medium">Xbox Controller</h1>
        </div>
        
        <div className="flex items-center gap-4">
          <div 
            className={cn(
              "px-3 py-1 rounded-full text-sm font-medium transition-all duration-300 flex items-center gap-2",
              isEmulationActive 
                ? "bg-green-500/10 text-green-500 dark:bg-green-500/20" 
                : "bg-secondary/80 text-muted-foreground dark:bg-zinc-800"
            )}
          >
            <div 
              className={cn(
                "h-2 w-2 rounded-full transition-colors duration-300", 
                isEmulationActive ? "bg-green-500 animate-pulse-subtle" : "bg-muted-foreground"
              )}
            ></div>
            {isEmulationActive ? "Ativo" : "Inativo"}
          </div>
          
          <button 
            onClick={toggleEmulation} 
            className="glass-button focus-effect"
          >
            {isEmulationActive ? "Desativar (F8)" : "Ativar (F8)"}
          </button>
          
          <button
            onClick={handleLogout}
            className="p-2 rounded-full hover:bg-secondary dark:hover:bg-zinc-800"
            title="Sair"
          >
            <LogOut size={18} />
          </button>
        </div>
      </div>
    </header>
  );
}
