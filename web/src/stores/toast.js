import { writable } from "svelte/store";

export const toast = writable({ show: false, message: "" });

export function showToast(message) {
  toast.set({ show: true, message });
  setTimeout(() => toast.set({ show: false, message: "" }), 3000);
}
