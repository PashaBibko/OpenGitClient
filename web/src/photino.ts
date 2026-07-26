interface PhotinoExternal {
    sendMessage: (message: string) => void;
    receiveMessage: (callback: (message: string) => void) => void;
}

export class PhotinoBridge {
    private static pendingResolve: ((message: string) => void) | null = null;
    private static queue: Promise<void> = Promise.resolve();

    private static Photino(): PhotinoExternal {
        return (window as unknown as { external: PhotinoExternal }).external;
    }

    private static ParseResponse(response: string): object | null {
        if (response == "null") {
            return null;
        }

        return JSON.parse(response);
    }

    public static Init() {
        this.Photino().receiveMessage((message: string): void => {
            if (this.pendingResolve) {
                const resolve: (message: string) => void = this.pendingResolve;
                this.pendingResolve = null;

                resolve(message);
            } else {
                console.warn("Message was received with no pending call")
            }
        });
    }

    public static CallBackend(funcName: string, params: object | null = null): Promise<object | null> {
        let message: string = funcName;
        if (params !== null) {
            message += '|' + JSON.stringify(params);
        }

        const result: Promise<object | null> = this.queue.then(() => this.SendAndWait(message));
        this.queue = result.then((): undefined => undefined, (): undefined => undefined);

        return result;
    }

    private static SendAndWait(message: string): Promise<object | null> {
        return new Promise<object | null>((resolve, reject) => {
            this.pendingResolve = (response: string) => {
                try {
                    resolve(this.ParseResponse(response));
                } catch (err) {
                    reject(err);
                }
            }

            this.Photino().sendMessage(message);
        })
    }

    // Disabled as it is a 'static' class
    // noinspection JSUnusedLocalSymbols
    private constructor() {}
}
