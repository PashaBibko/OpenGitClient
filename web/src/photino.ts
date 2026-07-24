interface PhotinoExternal {
    sendMessage: (message: string) => void;
}

export function CallBackend(funcName: string, params: object | null = null): void {
    const photino: PhotinoExternal = (window as unknown as { external: PhotinoExternal }).external;
    let message: string = funcName

    if (params !== null) {
        message += '|' + JSON.stringify(params)
    }

    photino.sendMessage(message);
}
