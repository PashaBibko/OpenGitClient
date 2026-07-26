import {CallBackend} from "./photino.ts";

export function App() {
    return (
        <div className="bg-neutral-800 min-h-screen">
            <h1>
                Open Git Client
            </h1>

            <button onClick={(): void => CallBackend("ReturnFunc", { value: 32 })}>
                Click me
            </button>
        </div>
    )
}
