
import { useEffect } from "react";
import { ControllerProvider } from "@/context/ControllerContext";
import { ThemeProvider } from "@/hooks/useTheme";
import { useKeyboardListener } from "@/hooks/useKeyboardListener";
import { useMouseListener } from "@/hooks/useMouseListener";
import { Nav } from "@/components/Nav";
import { ControllerVisual } from "@/components/ControllerVisual";
import { MappingConfig } from "@/components/MappingConfig";
import { SettingsPanel } from "@/components/SettingsPanel";
import { StatusBar } from "@/components/StatusBar";
import { useNavigate } from "react-router-dom";

const EmulatorContent = () => {
  useKeyboardListener();
  useMouseListener();
  const navigate = useNavigate();

  useEffect(() => {
    document.title = "Xbox Controller Emulator";
    
    // Check if user is authenticated
    const isAuthenticated = localStorage.getItem('isAuthenticated') === 'true';
    if (!isAuthenticated) {
      navigate('/login');
    }
  }, [navigate]);

  return (
    <div className="min-h-screen pt-24 pb-16 px-4 dark:bg-zinc-950">
      <Nav />
      
      <div className="container mx-auto space-y-8 animate-fade-in">
        <div>
          <span className="inline-block py-1 px-3 text-xs font-medium rounded-full bg-primary/10 text-primary mb-4">Visualização em Tempo Real</span>
          <h2 className="text-2xl font-medium mb-8">Estado do Controle</h2>
          <ControllerVisual />
        </div>
        
        <div className="grid md:grid-cols-2 gap-8 mt-12">
          <div>
            <span className="inline-block py-1 px-3 text-xs font-medium rounded-full bg-primary/10 text-primary mb-4">Teclado para Controle</span>
            <h2 className="text-2xl font-medium mb-6">Mapeamento de Botões</h2>
            <MappingConfig />
          </div>
          
          <div>
            <span className="inline-block py-1 px-3 text-xs font-medium rounded-full bg-primary/10 text-primary mb-4">Mouse para Analógico</span>
            <h2 className="text-2xl font-medium mb-6">Configurações de Sensibilidade</h2>
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
    <ThemeProvider>
      <ControllerProvider>
        <EmulatorContent />
      </ControllerProvider>
    </ThemeProvider>
  );
};

export default Index;
