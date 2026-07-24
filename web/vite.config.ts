import { viteSingleFile } from "vite-plugin-singlefile";
import { defineConfig } from "vite";

import tailwindcss from "@tailwindcss/vite";
import react from "@vitejs/plugin-react";

export default defineConfig(async () => ({
  plugins: [
    react(),
    tailwindcss(),
    viteSingleFile()
  ],
}));