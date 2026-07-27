import {PhotinoBridge} from "./photino.ts";
import {TopBar} from "./components/TopBar.tsx";

export function App() {
    return (
        <div className="bg-neutral-800 h-screen flex flex-col w-screen">
            <TopBar />

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
