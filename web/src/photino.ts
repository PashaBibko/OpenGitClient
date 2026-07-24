interface PhotinoExternal {
    sendMessage: (message: string) => void;
}

export function CallBackend(name: string): void {
    const photino: PhotinoExternal = (window as unknown as { external: PhotinoExternal }).external;
    photino.sendMessage(name);
}
