
import { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { Button } from '@/components/ui/button';
import { Input } from '@/components/ui/input';
import { useToast } from '@/components/ui/use-toast';

export default function Login() {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [isLoading, setIsLoading] = useState(false);
  const navigate = useNavigate();
  const { toast } = useToast();

  const handleLogin = (e: React.FormEvent) => {
    e.preventDefault();
    setIsLoading(true);
    
    // Simulating login - in a real app this would call an API
    setTimeout(() => {
      if (username === 'ADM' && password === 'ADM') {
        // Store auth state in localStorage
        localStorage.setItem('isAuthenticated', 'true');
        
        toast({
          title: "Login bem-sucedido",
          description: "Bem-vindo ao Emulador de Controle Xbox",
        });
        
        navigate('/');
      } else {
        toast({
          variant: "destructive",
          title: "Falha no login",
          description: "Nome de usuário ou senha incorretos.",
        });
      }
      setIsLoading(false);
    }, 1000);
  };

  return (
    <div className="h-screen w-full flex items-center justify-center bg-gradient-to-b from-background to-secondary/30 dark:from-background dark:to-secondary/10">
      <div className="max-w-md w-full mx-auto">
        <div className="glass-panel p-8 space-y-6">
          <div className="flex flex-col items-center space-y-2 mb-6">
            <div className="h-16 w-16 bg-zinc-900 rounded-full flex items-center justify-center mb-2">
              <svg xmlns="http://www.w3.org/2000/svg" width="40" height="40" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
                <path d="M6 9H4.5a2.5 2.5 0 0 1 0-5H6"></path>
                <path d="M18 9h1.5a2.5 2.5 0 0 0 0-5H18"></path>
                <path d="M4 22h16"></path>
                <path d="M10 14.66V17c0 .55-.47.98-.97 1.21C7.85 18.75 7 20.24 7 22"></path>
                <path d="M14 14.66V17c0 .55.47.98.97 1.21C16.15 18.75 17 20.24 17 22"></path>
                <path d="M18 2H6v7a6 6 0 0 0 12 0V2Z"></path>
              </svg>
            </div>
            <h1 className="text-2xl font-bold">Xbox Controller Emulator</h1>
            <p className="text-sm text-muted-foreground">Faça login para acessar o emulador</p>
          </div>
          
          <form onSubmit={handleLogin} className="space-y-4">
            <div className="space-y-2">
              <label htmlFor="username" className="text-sm font-medium">
                Nome de usuário
              </label>
              <Input
                id="username"
                type="text"
                value={username}
                onChange={(e) => setUsername(e.target.value)}
                className="glass-input w-full"
                required
              />
            </div>
            
            <div className="space-y-2">
              <label htmlFor="password" className="text-sm font-medium">
                Senha
              </label>
              <Input
                id="password"
                type="password"
                value={password}
                onChange={(e) => setPassword(e.target.value)}
                className="glass-input w-full"
                required
              />
            </div>
            
            <div className="pt-2">
              <Button 
                type="submit" 
                className="w-full font-semibold bg-zinc-900 hover:bg-black text-white"
                disabled={isLoading}
              >
                {isLoading ? "Entrando..." : "Entrar"}
              </Button>
            </div>
            
            <div className="text-center text-xs text-muted-foreground mt-4">
              <p>Use as credenciais padrão:</p>
              <p>Login: ADM / Senha: ADM</p>
            </div>
          </form>
        </div>
      </div>
    </div>
  );
}
