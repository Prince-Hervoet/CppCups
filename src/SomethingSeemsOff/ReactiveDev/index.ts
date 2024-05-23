function getProxyObject(obj: Object) {
  if (!obj) return {};
  return new Proxy(obj, {
    get(target: any, key: string) {
      console.log(key);
      return target[key];
    },
  });
}

function executor() {}

const test = getProxyObject({
  a: 1,
  get bar() {
    return this.a;
  },
});
console.log(test.bar);
