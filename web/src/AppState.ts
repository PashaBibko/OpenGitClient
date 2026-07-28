import {create} from 'zustand';

export interface WorkingDir {
    IsRepository: boolean,
    Filepath: string,
}

export interface AppState {
    CurrentWorkingDir: WorkingDir | null;
    setWorkingDir: (dir: WorkingDir) => void;
}

export const useAppState = create<AppState>((set) => ({
    CurrentWorkingDir: null,
    setWorkingDir: (dir) => set({ CurrentWorkingDir: dir })
}));
