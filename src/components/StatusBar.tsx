
import { useController } from '@/context/ControllerContext';
import { cn } from '@/lib/utils';
import { useTheme } from '@/hooks/useTheme';
import { Moon, Sun } from 'lucide-react';

export function StatusBar() {
  const { isEmulationActive } = useController();
  const { theme, setTheme } = useTheme();
  
  return (
    <div className="fixed bottom-0 left-0 right-0 py-2 px-4 bg-white/60 backdrop-blur-lg border-t border-white/20 shadow-sm z-40 dark:bg-black/60 dark:border-white/5">
      <div className="container mx-auto flex items-center justify-between">
        <div className="flex items-center gap-4">
          <div className="flex items-center gap-2">
            <div 
              className={cn(
                "h-2 w-2 rounded-full",
                isEmulationActive ? "bg-green-500 animate-pulse-subtle" : "bg-muted"
              )}
            ></div>
            <span className="text-sm">
              {isEmulationActive ? "Emulação Ativa" : "Emulação Inativa"}
            </span>
          </div>
          
          <div className="text-xs text-muted-foreground">
            Pressione F8 para alternar emulação
          </div>
        </div>
        
        <div className="flex items-center gap-4">
          <button 
            onClick={() => setTheme(theme === 'dark' ? 'light' : 'dark')}
            className="p-2 rounded-full hover:bg-secondary"
          >
            {theme === 'dark' ? <Sun size={18} /> : <Moon size={18} />}
          </button>
          
          <div className="text-xs text-muted-foreground">
            Emulador de Controle Xbox • v0.1.0
          </div>
        </div>
      </div>
    </div>
  );
}
