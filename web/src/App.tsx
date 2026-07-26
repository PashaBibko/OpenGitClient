import {PhotinoBridge} from "./photino.ts";

export function App() {
    return (
        <div className="bg-neutral-800 min-h-screen">
            <h1>
                Open Git Client
            </h1>

            <button onClick={(): void => {
                const returned: Promise<object | null> = PhotinoBridge.CallBackend("ReturnFunc");
                returned.then((result: object | null): void => {
                    console.log(result);
                })
                console.log(returned);
            }}>
                Click me
            </button>
        </div>
    )
}
