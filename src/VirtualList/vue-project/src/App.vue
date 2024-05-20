<script setup>
import VirtualList from "./VirtualList.vue";
</script>

<template>
  <div :style="{ width: '300px', height: '500px' }">
    <VirtualList
      :item-height="100"
      :view-height="500"
      :data="[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]"
      :buffer-count="2"
    >
      <template v-slot="item">
        <div
          :style="{
            backgroundColor: `blue`,
            boxSizing: `border-box`,
            padding: `10px`,
          }"
        >
          123123
          {{ item.data }}
        </div>
      </template>
    </VirtualList>
  </div>
</template>

<style scoped></style>
