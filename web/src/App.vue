<script setup lang="ts">
import { computed, onMounted, onUnmounted, ref } from 'vue'
import { createDevice, getDevices, removeDevice, updateDevice, type Device } from './api'

const devices = ref<Device[]>([])
const name = ref('')
const selectedId = ref('')
const connection = ref('连接中')
const errorMessage = ref('')
let socket: WebSocket | undefined
let reconnectTimer: number | undefined
const selectedDevice = computed(() => devices.value.find(item => item.id === selectedId.value))

async function refresh() {
  try {
    devices.value = await getDevices()
    if (selectedId.value && !devices.value.some(item => item.id === selectedId.value)) selectedId.value = ''
    errorMessage.value = ''
  } catch (error) {
    errorMessage.value = error instanceof Error ? error.message : '请求失败'
  }
}
async function add() {
  const value = name.value.trim()
  if (!value) return
  try { await createDevice(value); name.value = ''; await refresh() }
  catch (error) { errorMessage.value = error instanceof Error ? error.message : '新增失败' }
}
async function rename() {
  if (!selectedId.value || !name.value.trim()) return
  try { await updateDevice(selectedId.value, name.value.trim()); name.value = ''; await refresh() }
  catch (error) { errorMessage.value = error instanceof Error ? error.message : '修改失败' }
}
async function remove() {
  if (!selectedId.value) return
  try { await removeDevice(selectedId.value); selectedId.value = ''; await refresh() }
  catch (error) { errorMessage.value = error instanceof Error ? error.message : '删除失败' }
}
function connectSocket() {
  const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:'
  socket = new WebSocket(protocol + '//' + window.location.hostname + ':8081/ws')
  socket.onopen = () => { connection.value = '实时同步已连接' }
  socket.onmessage = () => { void refresh() }
  socket.onclose = () => {
    connection.value = '实时同步断开，正在重连'
    reconnectTimer = window.setTimeout(connectSocket, 2000)
  }
  socket.onerror = () => socket?.close()
}
onMounted(() => { void refresh(); connectSocket() })
onUnmounted(() => {
  if (reconnectTimer) window.clearTimeout(reconnectTimer)
  socket?.close()
})
</script>

<template>
  <main class="shell">
    <header class="topbar">
      <div>
        <div class="eyebrow">DEVICE OPERATIONS PLATFORM</div>
        <h1>DevLink</h1>
        <p>统一设备管理 · 桌面端与 Web 实时同步</p>
      </div>
      <div class="connection"><span class="dot"></span>{{ connection }}</div>
    </header>
    <section class="panel">
      <div class="panel-heading">
        <div><h2>设备管理</h2><p>通过统一后端管理虚拟设备，所有客户端共享同一份状态。</p></div>
        <button class="secondary" @click="refresh">刷新列表</button>
      </div>
      <div class="toolbar">
        <input v-model="name" placeholder="输入设备名称" @keyup.enter="add" />
        <button @click="add">新增设备</button>
        <button class="secondary" :disabled="!selectedDevice || !name.trim()" @click="rename">修改名称</button>
        <button class="danger" :disabled="!selectedDevice" @click="remove">删除设备</button>
      </div>
      <p v-if="errorMessage" class="error">{{ errorMessage }}</p>
      <div class="table-wrap">
        <table>
          <thead><tr><th>设备名称</th><th>设备类型</th><th>来源</th><th>状态</th><th>设备 ID</th></tr></thead>
          <tbody>
            <tr v-for="device in devices" :key="device.id"
                :class="{ selected: selectedId === device.id }"
                @click="selectedId = device.id; name = device.name">
              <td class="device-name">{{ device.name }}</td>
              <td>{{ device.type }}</td>
              <td>{{ device.source === 'virtual' ? '模拟设备' : '物理设备' }}</td>
              <td><span class="status">{{ device.status }}</span></td>
              <td class="id">{{ device.id }}</td>
            </tr>
            <tr v-if="devices.length === 0"><td colspan="5" class="empty">暂无设备，请先新增一台设备。</td></tr>
          </tbody>
        </table>
      </div>
      <footer>{{ devices.length }} 台设备 <span>选择设备后可修改名称或删除</span></footer>
    </section>
    <div class="footnote">DevLink 0.1.0 · 当前为局域网开发原型，尚未启用身份认证与数据持久化</div>
  </main>
</template>
