export interface Device {
  id: string
  name: string
  type: string
  source: 'virtual' | 'physical'
  status: 'offline' | 'online' | 'warning' | 'error'
  createdAt: string
  updatedAt: string
}
const API_BASE = '/api/v1'
export async function getDevices(): Promise<Device[]> {
  const response = await fetch(API_BASE + '/devices')
  if (!response.ok) throw new Error('HTTP ' + response.status)
  const payload = await response.json() as { devices: Device[] }
  return payload.devices
}
export async function createDevice(name: string): Promise<void> {
  const response = await fetch(API_BASE + '/devices', {
    method: 'POST', headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ name, type: 'generic', source: 'virtual' }),
  })
  if (!response.ok) throw new Error('HTTP ' + response.status)
}
export async function updateDevice(id: string, name: string): Promise<void> {
  const response = await fetch(API_BASE + '/devices/' + encodeURIComponent(id), {
    method: 'PUT', headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ name }),
  })
  if (!response.ok) throw new Error('HTTP ' + response.status)
}
export async function removeDevice(id: string): Promise<void> {
  const response = await fetch(API_BASE + '/devices/' + encodeURIComponent(id), { method: 'DELETE' })
  if (!response.ok) throw new Error('HTTP ' + response.status)
}
